function [M, E, TM, TE, Vi0, positions, ...
    qLTM, qLTE, projec, etasM, AtOrVt] = proproc(nN, vertexmat, ntags, nEm1, ... % mesh information
    nEm2, nEm3, nEm4, nEm5, nEm6, nEm7, nEm15, ... % mesh information
    dt, mediumtags, ro, cp, k, Ti, wb, pb, cb, Tb, ... % problem's information
    Qmet, boundarytags, boundarytype, boundaryvalues) % problem's information
% This file is part of TLMBHT. See details of this function below.
% Version 0.1. Copyright © 2015 to 2016 by Cornell University.
% Written by Hugo Fernando Maia Milan.
% All Rights Reserved. Refer to the license file for details.
%
% Acknowledgment: Brazilian National Counsel of Technological and Scientific
% Development (Proc. 203312/2014-7) for scholarship to HFMM
%
%
%
%
%
%
% This algorithm assumes that the medium is composed of tetrahedrons and/or 
% triangles and boundaries are composed of triangles and/or lines. Other 
% arrangements are possible but they were not implemented yet.
%
% The workflow is the following. 1) start with the node that has the
% highest number of vertices (read degrees of freedom here), 2) calculate
% the scattering matrix, Resistances, impedances and associates;
% in parallel, calculate boundary conditions; 3) discover what are the intersection points
% for the ports in the nodes, 4) store appropriated values, 5) moves to
% the next node. 6) When scattering is done, start again to calculate
% the connection matrix including boundary condutions.
%
%
%
%%%%%%%%%%%%%%%% Inputs %%%%%%%%%%%%%%
%
% nN-nEm15: mesh data got from openmsh.m. See openmsh.m for
% more information.
%
% dt: time-step.
%
% mediumtags: tags number for the different mediums, it should be a vector
% in column format where each column represent a different medium.
%
% ro: density (kg/m3) of each medium (should be a vector in column form).
%
% cp: specific heat (J/(K-kg)) of each medium (should be a vector in column form).
%
% k: thermal conductivity (W/(m-K)) of each medium (should be a vector in column form).
%
% Ti: initial temperature (oC) of each medium (should be a vector in a column form).
%
% wb: blood perfusion (s-1) of each medium (should be a vector in a column form).
%
% pb: blood density (kg/m3) of each medium (should be a vector in a column form).
%
% cb: blood specific heat (J/(K-kg)) of each medium (should be a vector in a column form).
%
% Tb: blood temperature (oC) of each medium (should be a vector in a column form).
%
% Qmet: total (metabolic + external) heat generation (W/m3) of each medium (should be a vector in a column form).
%
% boundarytags: similar to medium tags. Tags number for the different boundaries, it should be a vector
% in column format where each column represent a different boundary.
%
% boundarytype: column vector where each column is a flag telling the
% algorithm what kind of boundary condition the boundarytags is. 1 - temperature;
% 2 - heat flux; 3 - Newtons cooling law.
%
% boundaryvalues: matrix that contains the pertinent values for each
% boundary type. For example, for 1 it contains the temperature at the
% boundary.








% here we will calculate the number of incident/reflected voltages. This is,
% we will find out what the size of our connection and scattering matrix is
% going to be. Note that we are restricted to the mediums defined by the
% user. If the user did not define a medium, there is no guarantee that the code
% will work. The user MUST defined all the mediums that he/she is going to use.
% Mediums without definition are just ignored.
szmt = 0; % this is the total number of ports
szt = 0; % one temperature per nodes' center, then, it is the total number of nodes
for nv = 1:size(mediumtags,2)
    for nt = 1:size(ntags,1)
        if mediumtags(nv) == ntags(nt,1)
            szmt = szmt + ntags(nt,2)*2 + ntags(nt,3)*3 + ntags(nt,4)*4 + ...
                ntags(nt,5)*4 + ntags(nt,6)*6 + ntags(nt,7)*5 + ntags(nt,8)*5; % add the number of ports
            % Columns in the ntags and meaning.
            % 2: 2-node line (2 ports). 3: 3-node triangle (3 ports, or faces).
            % 4: 4-node quadrangle (4 ports, or faces). 5: 4-node tetrahedron (4 ports, or faces).
            % 6: 8-node hexahedron (6 ports, or faces). 7: 6-node prism (5 ports, or faces).
            % 8: 5-node pyramid (5 ports, or faces). 9: 1-node point.
            
            szt = szt + sum(ntags(nt,2:8)); % add one temperature per element type
            break
        end
    end
end

% The matrix equation that we have to solve is
% (k+1)Vi = M*(k)Vi + E
% used for calculating (k+1)Vi with the values of (k)Vi.
% M = C*S
C = sparse(1,1,0,szmt,szmt,szmt*2); % Connection matrix. Temporary variable
S = sparse(1,1,0,szmt,szmt,szmt*4); % Scattering matrix. Temporary variable


% constant used for calculating (k+1)Vi. It has the influnce of external
% heat sources and boundary conditions
E = zeros(szmt,1);
% E = C*ZIS + B
ZIS = zeros(szmt,1);% External sources on node's temperature. Temporaly variable

% if we are not doing the math for steady state, then we need an initial
% value for Vi
Vi0 = zeros(szmt,1);

% For Tc (temperature in the node's center):
% (k)Tc = tau*(k)Vi + ZIS
% For Tp (temperature in the node's ports):
% (k)Tp = beta*(k)Vr + TB. We can take both matrixes together to get:
% (k)T = TM*(k)Vi + TE
TM = sparse(1,1,0,szt,szmt,szt*3); % calculating (k)T based on the values of (k)Vi
% TM = tau + beta*S; beta here is appended to tau. They really do not
% sum up together

% constant used for calculating (k)T. It has the influce of external
% heat sources and boundary conditions
TE = zeros(szt,1);
% TE = ZIS1 + beta*ZIS + TB; ZIS1 is a matrix in the size of Tc. The
% same happens here, beta*ZIS + TB is appended to ZIS1.

% used to calculate the eta value, so that we can verify if our
% approximation for the Fourier's conduction law is good
etas = zeros(szmt,1);







% position of the nodes and ports
posimat = zeros(szt,3); % this contains x, y, z for each TLM node. If any,
% temperature or heat flux, is going to be calculate in the ports, then,
% the positions in x, y, z for the ports will be appended to this variable.
posimatports = zeros(szmt,3); % this contains x, y, z for each TLM port.
AtOrVt = zeros(szt,1); % contains area of triangles (if nodes) and volume of tetrahedrons (if nodes). Used for debug

% variable used to calculate the matrixes. Temporary
projections = zeros(szmt,6); % this contains lL,lLt, L, projection_in_x, projection_in_y, projection_in_z for each TLM port; ln is the projection in that direction
nPort2nNode = zeros(szmt,1); % vector that stores the number of the node given the number of the port


% vectors that stores the values for Zall and Rall. They are required
% because we need to calculate Z and R, find how the mesh interconects,
% and, finally, use Z and R to calculate the connection matrix.
Zall = zeros(szmt,1);
Rall = zeros(szmt,1);





% the lines of IPnPorts is the number of the intersection point.
% the 1st and 2nd column have the value correspondent to
% the port numbers that are connected to that specific intersection point.

% The 3rd column tells if this intersection point is going to have a
% boundary condition. 0 indicates no boundary condition, -1 indicates 
% temperature as boundary condition, -2 indicates heat flux as boundary
% condition, -3 indicates Newton's cooling law as boundary condition

% 4th and 5th columns contains values specific for BC.
% for -1: 4th = Temp; 5th = 0;
% for -2: 4th = qflux; 5th = 0;
% for -3: 4th = Temp_inf; 5th = h (heat transfer coefficient).


IPnPorts = zeros(szmt,5);
IP = 1; % the first number of the intersection point. This variable will be
% picked up when we need to declare a new intersection number. Then, at the
% end of this algorithm, the total number of intersection points is IP - 1.
% IPm = zeros(nN, nN, nN);
IPm = sparse(1, 1, 0, nN, nN*nN, 2*nN); % this matrix contains the values of the intersection points
% IPm = sparse(1, 1, 0, nN, nN, 2*nN); % this matrix contains the values of the intersection points
% for each pair of vertices. Vertices in the lines are smaller than vertices in the
% column. nN is the total number of vertices in the problem.

% the overal logic for the intersection points is the following. 1) get the
% number of the vertices for a particular node. 2) go to the IPm with those vertices
% and see if there is an intersection point for these vertices. If there is,
% then get that intersection point and goes to Zeq. If not, then, create a
% new intersection point.


% these variables are used as an accumulation factor.
% nNodeAcu is the actual node being processed.
% nPortsAcu is the first number of the port being processed.
% nPortsAcuEnd is the last number of the port being processed.
% For tetrahedrons, nPortsAcuEnd = nPortsAcu + 3;
% For triangles, nPortsAcuEnd = nPortsAcu + 2;
nNodeAcu = 1; nPortsAcu = 1;




% tetrahedral nomenclature.
%                      vertex 1
%                          __
%                         /\ \_
%                        /  \  \_
%                       /    \   \_
%                      /      \    \_  edge 5
%                     /        \     \_
%                    /          \      \_
%                   /          ar\ea 3   \_
%                  /              \        \_
%         edge 3  /                \  area 2 \_
%                /      area 4      \ edge 4   \_  vertex 4
%               /                    \    _____//
%              /                  ____\__/     /
%             /     edge 6 ______/     \      / edge 1
%            /      ______/             \    /
%           /______/           area 1    \  /
% vertex 2 //_____________________________\/ vertex 3
%                      edge 2
%
% area n has all vertex m, which m ~= n. The nomenclature for edges is more
% obscure. We assumed that:
% vertex 1 and vertex 2: edge 3
% vertex 1 and vertex 3: edge 4
% vertex 1 and vertex 4: edge 5
% vertex 4 and vertex 2: edge 6
% vertex 4 and vertex 3: edge 7
% vertex 3 and vertex 2: edge 2
% Therefore:
% area 1 has edges 1, 2, 6
% area 2 has edges 1, 4, 5
% area 3 has edges 3, 5, 6
% area 4 has edges 2, 3, 4


nPortsAcuEnd = nPortsAcu + 3; % accumulation factor for the ports. This is the
% last port being processed.


% Here, we make a loop for the tetrahedral elements and do the calculations
% for them.

% temporary variables for the tetrahedral
deltaX = zeros(1,6); deltaY = zeros(1,6); deltaZ = zeros(1,6);
A = zeros(1,4);
deltaXl = zeros(1,4); deltaYl = zeros(1,4); deltaZl = zeros(1,4);
deltal = zeros(1,4);
x_p = zeros(1,4); y_p = zeros(1,4); z_p = zeros(1,4);
Abs_p = zeros(1,4);

for varE4 = 0:(size(nEm4,1)-1)
    
    tag_phy = nEm4(varE4+1,1); % getting the number of the physical tag.
    % the physical tag is used for defining mediums and boundary condition
    
    % verifying if this element is medium
    for tagsfor = 1:size(mediumtags,2)
        % The element is a medium.
        if tag_phy == mediumtags(tagsfor)
            nPort2nNode(nPortsAcu:nPortsAcuEnd) = nNodeAcu;
            
            
            % tagsfor is the number of the medium that has the parameters
            % we want.
            
            % Element's vertex
            vertexn1 = nEm4(varE4+1,2); % number of vertex 1
            vertexn2 = nEm4(varE4+1,3); % number of vertex 2
            vertexn3 = nEm4(varE4+1,4); % number of vertex 3
            vertexn4 = nEm4(varE4+1,5); % number of vertex 3
            
            
            
            % Gmsh gives the values in mm. Here we work in m. Then, we need
            % to multiply the values of the vertex's position by 1e-3.
            vertex1 = vertexmat(vertexn1,:)*1e-3; % vertex 1 (x, y, z) in mm
            vertex2 = vertexmat(vertexn2,:)*1e-3; % vertex 2 (x, y, z) in mm
            vertex3 = vertexmat(vertexn3,:)*1e-3; % vertex 3 (x, y, z) in mm
            vertex4 = vertexmat(vertexn4,:)*1e-3; % vertex 4 (x, y, z) in mm
            
            % distance and lengths for edges
            % edge 1
            deltaX(1) = vertex4(1) - vertex3(1);
            deltaY(1) = vertex4(2) - vertex3(2);
            deltaZ(1) = vertex4(3) - vertex3(3);
            % edge 2
            deltaX(2) = vertex3(1) - vertex2(1);
            deltaY(2) = vertex3(2) - vertex2(2);
            deltaZ(2) = vertex3(3) - vertex2(3);
            % edge 3
            deltaX(3) = vertex1(1) - vertex2(1);
            deltaY(3) = vertex1(2) - vertex2(2);
            deltaZ(3) = vertex1(3) - vertex2(3);
            % edge 4
            deltaX(4) = vertex1(1) - vertex3(1);
            deltaY(4) = vertex1(2) - vertex3(2);
            deltaZ(4) = vertex1(3) - vertex3(3);
            % edge 5
            deltaX(5) = vertex1(1) - vertex4(1);
            deltaY(5) = vertex1(2) - vertex4(2);
            deltaZ(5) = vertex1(3) - vertex4(3);
            % edge 4
            deltaX(6) = vertex4(1) - vertex2(1);
            deltaY(6) = vertex4(2) - vertex2(2);
            deltaZ(6) = vertex4(3) - vertex2(3);
            
            % Triangle's area
            % area 1
            A(1) = sqrt(  ( deltaY(1)*deltaZ(2) - deltaY(2)*deltaZ(1) )^2 + ...
                ( deltaX(1)*deltaZ(2) - deltaX(2)*deltaZ(1) )^2 + ...
                ( deltaX(1)*deltaY(2) - deltaX(2)*deltaY(1) )^2   )/2;
            % area 2
            A(2) = sqrt(  ( deltaY(1)*deltaZ(4) - deltaY(4)*deltaZ(1) )^2 + ...
                ( deltaX(1)*deltaZ(4) - deltaX(4)*deltaZ(1) )^2 + ...
                ( deltaX(1)*deltaY(4) - deltaX(4)*deltaY(1) )^2   )/2;
            % area 3
            A(3) = sqrt(  ( deltaY(3)*deltaZ(5) - deltaY(5)*deltaZ(3) )^2 + ...
                ( deltaX(3)*deltaZ(5) - deltaX(5)*deltaZ(3) )^2 + ...
                ( deltaX(3)*deltaY(5) - deltaX(5)*deltaY(3) )^2   )/2;
            % area 4
            A(4) = sqrt(  ( deltaY(3)*deltaZ(2) - deltaY(2)*deltaZ(3) )^2 + ...
                ( deltaX(3)*deltaZ(2) - deltaX(2)*deltaZ(3) )^2 + ...
                ( deltaX(3)*deltaY(2) - deltaX(2)*deltaY(3) )^2   )/2;
            
            % Barycenter
            posimat(nNodeAcu,1:3) = mean([vertex1; vertex2; vertex3; vertex4]);
            % here we store the position of the current ports
            posimatports(nPortsAcu,:) = mean([vertex2; vertex3; vertex4]);
            posimatports(nPortsAcu + 1,:) = mean([vertex1; vertex3; vertex4]);
            posimatports(nPortsAcu + 2,:) = mean([vertex1; vertex2; vertex4]);
            posimatports(nPortsAcuEnd,:) = mean([vertex1; vertex2; vertex3]);
            
            % Distance from ports to barycenter.
            % face 1
            deltaXl(1) = posimat(nNodeAcu,1) - posimatports(nPortsAcu,1);
            deltaYl(1) = posimat(nNodeAcu,2) - posimatports(nPortsAcu,2);
            deltaZl(1) = posimat(nNodeAcu,3) - posimatports(nPortsAcu,3);
            deltal(1) = sqrt(deltaXl(1)^2 + deltaYl(1)^2 + deltaZl(1)^2);
            % face 2
            deltaXl(2) = posimat(nNodeAcu,1) - posimatports(nPortsAcu + 1,1);
            deltaYl(2) = posimat(nNodeAcu,2) - posimatports(nPortsAcu + 1,2);
            deltaZl(2) = posimat(nNodeAcu,3) - posimatports(nPortsAcu + 1,3);
            deltal(2) = sqrt(deltaXl(2)^2 + deltaYl(2)^2 + deltaZl(2)^2);
            % face 3
            deltaXl(3) = posimat(nNodeAcu,1) - posimatports(nPortsAcu + 2,1);
            deltaYl(3) = posimat(nNodeAcu,2) - posimatports(nPortsAcu + 2,2);
            deltaZl(3) = posimat(nNodeAcu,3) - posimatports(nPortsAcu + 2,3);
            deltal(3) = sqrt(deltaXl(3)^2 + deltaYl(3)^2 + deltaZl(3)^2);
            % face 4
            deltaXl(4) = posimat(nNodeAcu,1) - posimatports(nPortsAcuEnd,1);
            deltaYl(4) = posimat(nNodeAcu,2) - posimatports(nPortsAcuEnd,2);
            deltaZl(4) = posimat(nNodeAcu,3) - posimatports(nPortsAcuEnd,3);
            deltal(4) = sqrt(deltaXl(4)^2 + deltaYl(4)^2 + deltaZl(4)^2);
            
            % Volume of the tetrahedron
            % V = 1/3 x An x height
            % we will use A1, then height is the distance from A1 center to
            % the vertex1. To calculate the height, we will first find the vector
            % perpendicular to the plan of A1
            x_p(1) = deltaY(1)*deltaZ(2) - deltaY(2)*deltaZ(1);
            y_p(1) = deltaZ(1)*deltaX(2) - deltaZ(2)*deltaX(1);
            z_p(1) = deltaX(1)*deltaY(2) - deltaX(2)*deltaY(1);
            Abs_p(1) = sqrt( x_p(1)^2 + y_p(1)^2 + z_p(1)^2 );
            
            % in addition, we will also calculate the next perpendicular
            % vectors, since they will be to calculate qLT in the
            % validation
            x_p(2) = deltaY(1)*deltaZ(4) - deltaY(4)*deltaZ(1);
            y_p(2) = deltaZ(1)*deltaX(4) - deltaZ(4)*deltaX(1);
            z_p(2) = deltaX(1)*deltaY(4) - deltaX(4)*deltaY(1);
            Abs_p(2) = sqrt( x_p(2)^2 + y_p(2)^2 + z_p(2)^2 );
            
            x_p(3) = deltaY(3)*deltaZ(5) - deltaY(5)*deltaZ(3);
            y_p(3) = deltaZ(3)*deltaX(5) - deltaZ(5)*deltaX(3);
            z_p(3) = deltaX(3)*deltaY(5) - deltaX(5)*deltaY(3);
            Abs_p(3) = sqrt( x_p(3)^2 + y_p(3)^2 + z_p(3)^2 );
            
            x_p(4) = deltaY(3)*deltaZ(2) - deltaY(2)*deltaZ(3);
            y_p(4) = deltaZ(3)*deltaX(2) - deltaZ(2)*deltaX(3);
            z_p(4) = deltaX(3)*deltaY(2) - deltaX(2)*deltaY(3);
            Abs_p(4) = sqrt( x_p(4)^2 + y_p(4)^2 + z_p(4)^2 );
            
            % now we calculate the volume of the tetrahedron
            
            Vt = 1/3*A(1)*abs(   x_p(1)*( vertex1(1) - vertex3(1) ) + ...
                y_p(1)*( vertex1(2) - vertex3(2) ) + ...
                z_p(1)*( vertex1(3) - vertex3(3) )   )/Abs_p(1);
            
            AtOrVt(nNodeAcu) = Vt;
            
            % now we will define the projections lL and lLT.
            % projections lL
%             projections(nPortsAcu:nPortsAcuEnd,1) = (   ( deltaX.*deltaXl + ...
%                 deltaY.*deltaYl + deltaZ.*deltaZl )./( L.*deltal )   )';
            
            % projections lLT. These are for 2D plane. For the 3D plane it
            % is more complicated and will be inserted here later
%             projections(nPortsAcu:nPortsAcuEnd,2) = (   ( -deltaY.*deltaXl + ...
%                 deltaX.*deltaYl )./( L.*deltal )   )'; % this projection shall be posite, always
            
            % testing projections direction. Here we will test if the
            % direction of the vector perpendicular to the area An points
            % toward the node center. If it does not, we will multiply it
            % by -1.
            for a = 1:4
                if (x_p(a)*deltaXl(a) + y_p(a)*deltaYl(a) + z_p(a)*deltaZl(a)) < 0
                    x_p(a) = - x_p(a);
                    y_p(a) = - y_p(a);
                    z_p(a) = - z_p(a);
                end
%                 cross([x_p(a) y_p(a) z_p(a)], ...
%                 [deltaXl(a) deltaYl(a) deltaZl(a)])
%             (x_p(a)*deltaXl(a) + y_p(a)*deltaYl(a) + z_p(a)*deltaZl(a))/(Abs_p(a)*deltal(a))
%                 pause
            end
            
            % this is A
            projections(nPortsAcu:nPortsAcuEnd,3) = A';
            % projections lx
            projections(nPortsAcu:nPortsAcuEnd,4) = (x_p ./Abs_p)';%(deltaXl ./deltal)';
%             projections(nPortsAcu:nPortsAcuEnd,4) = (deltaXl ./deltal)';
            % projections ly
            projections(nPortsAcu:nPortsAcuEnd,5) = (y_p ./Abs_p)';%(deltaYl ./deltal)';
%             projections(nPortsAcu:nPortsAcuEnd,5) = (deltaYl ./deltal)';
            % projections lz
            projections(nPortsAcu:nPortsAcuEnd,6) = (z_p ./Abs_p)';%(deltaZl ./deltal)';
%             projections(nPortsAcu:nPortsAcuEnd,6) = (deltaZl ./deltal)';
            
            
            etas(nPortsAcu:nPortsAcuEnd) = (A./(deltal.^2))'*k(tagsfor)/...
                (ro(tagsfor)*cp(tagsfor))*dt*sum(deltal)/(4*Vt);
            
            
            % Capacitance of the model
            Cap = Vt*ro(tagsfor)*cp(tagsfor)/sum(deltal); % new model
            
            % Impedances of the model
            Z = dt./(2*Cap.*deltal);
            Zall(nPortsAcu:nPortsAcuEnd) = Z';
            
            % resistances of the model
            R = deltal./( k(tagsfor)*A);
            Rall(nPortsAcu:nPortsAcuEnd) = R';
            
            % conductance of the model
            G = Vt*wb(tagsfor)*pb(tagsfor)*cb(tagsfor);
            
            % current source of the model
            Is = Vt*( Qmet(tagsfor) + wb(tagsfor)*pb(tagsfor)*cb(tagsfor)*Tb(tagsfor) );
            
            % Zhat
            Zhat = Z(1)*Z(2)*Z(3)*Z(4)/( Z(1)*Z(2)*Z(3) + Z(1)*Z(2)*Z(4) + ...
                Z(1)*Z(3)*Z(4) + Z(2)*Z(3)*Z(4) + Z(1)*Z(2)*Z(3)*Z(4)*G );
            
            % transmitances
            tau = 2*Zhat./Z;
            
            % scattering matrix. From Vi to Vr. This is a process that
            % occurs just into the node. It does not depend from
            % anything outside the node. This is a temporarly variable
            S( nPortsAcu:nPortsAcuEnd, nPortsAcu:nPortsAcuEnd ) = ...
                [ ( tau(1) - 1 ),     tau(2),             tau(3),       tau(4); ...
                tau(1),         ( tau(2) - 1 ),       tau(3),           tau(4); ...
                tau(1),           tau(2),           ( tau(3) - 1 ),     tau(4);...
                tau(1),           tau(2),             tau(3),         ( tau(4) - 1)];
            
            
            % effect of external sources on Vr calculation. This is a
            % temporary variable.
            ZIS( nPortsAcu:nPortsAcuEnd) = [1; 1; 1; 1]*Zhat*Is;
            
            % These matrixes are used when we want to calculate the
            % temperature at node's center.
            %             if CalculateTc == 1
            TM( nNodeAcu, nPortsAcu:nPortsAcuEnd ) = tau;
            TE( nNodeAcu ) = ZIS(nPortsAcu);
            %             end
            
            % if we are not solving for steady-state, we need to calculate
            % the initial value for the incidente voltage
            %             if (SteadyState ~= 1)
            Vi0( nPortsAcu:nPortsAcuEnd) = ( Ti(tagsfor) - ZIS(nPortsAcu) )/sum(tau);
            %             end
            
            
            
            
            
            % here we implement the intersection point
            % The vertex that represents the line must be always lower than
            % the vertex that represents the column
            for convar = 0:3
                switch convar
                    case 0
                        vertex = sort([vertexn2 vertexn3 vertexn4]);
                    case 1
                        vertex = sort([vertexn1 vertexn3 vertexn4]);
                    case 2
                        vertex = sort([vertexn1 vertexn2 vertexn4]);
                    case 3
                        vertex = sort([vertexn1 vertexn2 vertexn3]);
                end
                
                
                % Is there any intersection point allocated here?
                IPt = IPm(vertex(1),vertex(2) + (vertex(3) - 1)*nN); % intersection point being tested
                if IPt ~= 0 % yes
                    IPnPorts(IPt,2) = nPortsAcu + convar; % here we allocate
                    % the number of the port in that intersection point
                    
                else
                    IPm(vertex(1), vertex(2) + (vertex(3) - 1)*nN) = IP; % call this the number of the variable IP
                    
                    IPnPorts(IP,1) = nPortsAcu + convar; % here we allocate
                    % the number of the port in that intersection point
                    IP = IP + 1; % increase the intersection point by 1
                end
                
            end
            
            
            
            
            % now we increase nNodeAcu by 1; nPortsAcu by 4 (for 4 ports in
            % the tetrahedral model). nPortsAcuEnd by three (for 4 ports in the
            % tetrahedral model).
            nNodeAcu = nNodeAcu + 1; nPortsAcu = nPortsAcu + 4; nPortsAcuEnd = nPortsAcu + 3;
            break
        end
    end
end



% triangular nomenclature.
%              vertex 1
%              /\
%     face3   /  \   face 2
%            /    \
%           /      \
% vertex 2 /________\ vertex 3
%            face 1

% triangles can be boundary conditions and mediums. The boundary conditions
% version was not implemented yet.

nPortsAcuEnd = nPortsAcu + 2; % accumulation factor for the ports. This is the
% last port being processed.


% Here, we make a loop for the triangles elements and do the calculations
% for them.

% temporary variables for the triangle
deltaX = zeros(1,3); deltaY = zeros(1,3); deltaZ = zeros(1,3);
L = zeros(1,3);
deltaXl = zeros(1,3); deltaYl = zeros(1,3); deltaZl = zeros(1,3);
deltal = zeros(1,3);
for varE2 = 0:(size(nEm2,1)-1)
    
    tag_phy = nEm2(varE2+1,1); % getting the number of the physical tag.
    % the physical tag is used for defining mediums and boundary conditions
    
    medium = -1;
    % verifying if this element is medium
    for tagsfor = 1:size(mediumtags,2)
        if tag_phy == mediumtags(tagsfor)
            medium = 1;
            break
        end
    end
    
    % verifying if this element is boundary
    if medium ~= 1
    for tagsfor = 1:size(boundarytags,2)
        if tag_phy == boundarytags(tagsfor)
            medium = 0;
            break
        end
    end
    end
    % The element is a medium.
    if medium == 1
        nPort2nNode(nPortsAcu:nPortsAcuEnd) = nNodeAcu;
        
        % tagsfor is the number of the medium that has the parameters
        % we want.
        
        % Element's vertex
        vertexn1 = nEm2(varE2+1,2); % number of vertex 1
        vertexn2 = nEm2(varE2+1,3); % number of vertex 2
        vertexn3 = nEm2(varE2+1,4); % number of vertex 3
        
        
        
        % Gmsh gives the values in mm. Here we work in m. Then, we need
        % to multiply the values of the vertex's position by 1e-3.
        vertex1 = vertexmat(vertexn1,:)*1e-3; % vertex 1 (x, y, z) in mm
        vertex2 = vertexmat(vertexn2,:)*1e-3; % vertex 2 (x, y, z) in mm
        vertex3 = vertexmat(vertexn3,:)*1e-3; % vertex 3 (x, y, z) in mm
        
        % distances and lengths for the triangles faces
        % face 1
        deltaX(1) = vertex2(1) - vertex3(1);
        deltaY(1) = vertex2(2) - vertex3(2);
        deltaZ(1) = vertex2(3) - vertex3(3);
        L(1) = sqrt(deltaX(1)^2 + deltaY(1)^2 + deltaZ(1)^2);
        % face 2
        deltaX(2) = vertex1(1) - vertex3(1);
        deltaY(2) = vertex1(2) - vertex3(2);
        deltaZ(2) = vertex1(3) - vertex3(3);
        L(2) = sqrt(deltaX(2)^2 + deltaY(2)^2 + deltaZ(2)^2);
        % face 3
        deltaX(3) = vertex1(1) - vertex2(1);
        deltaY(3) = vertex1(2) - vertex2(2);
        deltaZ(3) = vertex1(3) - vertex2(3);
        L(3) = sqrt(deltaX(3)^2 + deltaY(3)^2 + deltaZ(3)^2);
        
        % Triangle's area.
        At = sqrt(  ( deltaY(1)*deltaZ(2) - deltaY(2)*deltaZ(1) )^2 + ...
            ( deltaX(1)*deltaZ(2) - deltaX(2)*deltaZ(1) )^2 + ...
            ( deltaX(1)*deltaY(2) - deltaX(2)*deltaY(1) )^2   )/2;
        
        
        
        % Barycenter
        posimat(nNodeAcu,1:3) = mean([vertex1; vertex2; vertex3]);
        AtOrVt(nNodeAcu) = At;
        % here we store the position of the current port
        posimatports(nPortsAcu,:) = mean([vertex2; vertex3]);
        posimatports(nPortsAcu + 1,:) = mean([vertex1; vertex3]);
        posimatports(nPortsAcuEnd,:) = mean([vertex2; vertex1]);
        
        % Distance from the ports to the barycenter.
        % face 1
        deltaXl(1) = posimat(nNodeAcu,1) - ( vertex2(1) + vertex3(1) )/2;
        deltaYl(1) = posimat(nNodeAcu,2) - ( vertex2(2) + vertex3(2) )/2;
        deltaZl(1) = posimat(nNodeAcu,3) - ( vertex2(3) + vertex3(3) )/2;
        deltal(1) = sqrt(deltaXl(1)^2 + deltaYl(1)^2 + deltaZl(1)^2);
        % face 2
        deltaXl(2) = posimat(nNodeAcu,1) - ( vertex1(1) + vertex3(1) )/2;
        deltaYl(2) = posimat(nNodeAcu,2) - ( vertex1(2) + vertex3(2) )/2;
        deltaZl(2) = posimat(nNodeAcu,3) - ( vertex1(3) + vertex3(3) )/2;
        deltal(2) = sqrt(deltaXl(2)^2 + deltaYl(2)^2 + deltaZl(2)^2);
        % face 3
        deltaXl(3) = posimat(nNodeAcu,1) - ( vertex1(1) + vertex2(1) )/2;
        deltaYl(3) = posimat(nNodeAcu,2) - ( vertex1(2) + vertex2(2) )/2;
        deltaZl(3) = posimat(nNodeAcu,3) - ( vertex1(3) + vertex2(3) )/2;
        deltal(3) = sqrt(deltaXl(3)^2 + deltaYl(3)^2 + deltaZl(3)^2);
        
        % now we will define the projections lL and lLT.
        % projections lL
        projections(nPortsAcu:nPortsAcuEnd,1) = (   ( deltaX.*deltaXl + ...
            deltaY.*deltaYl + deltaZ.*deltaZl )./( L.*deltal )   )';
        
        % projections lLT. These are for 2D plane. For the 3D plane it
        % is more complicated and will be inserted here later
        projections(nPortsAcu:nPortsAcuEnd,2) = (   ( -deltaY.*deltaXl + ...
            deltaX.*deltaYl )./( L.*deltal )   )'; % this projection shall be posite, always
        
        for a = 0:2
            if projections(nPortsAcu + a,2) < 0
                projections(nPortsAcu + a,2) = -projections(nPortsAcu + a,2);
                deltaY(a + 1) = - deltaY(a + 1);
                deltaX(a + 1) = - deltaX(a + 1);
            end
        end
        
        % this is L
        projections(nPortsAcu:nPortsAcuEnd,3) = L';
        % projections lx
        projections(nPortsAcu:nPortsAcuEnd,4) = (-deltaY ./L)';
        % projections ly
        projections(nPortsAcu:nPortsAcuEnd,5) = (deltaX ./L)';
        
        
        etas(nPortsAcu:nPortsAcuEnd) = (L./(deltal.^2))'*k(tagsfor)/...
            (ro(tagsfor)*cp(tagsfor))*dt*sum(deltal)/(4*At);
        
        
        % Capacitance of the model
        Cap = At*ro(tagsfor)*cp(tagsfor)/sum(deltal); % new model
        
        % Impedances of the model
        Z = dt./(2*Cap.*deltal);
        Zall(nPortsAcu:nPortsAcuEnd) = Z';
        
        % resistances of the model
        R = deltal./( k(tagsfor)*L);%*projections(nPortsAcu:nPortsAcuEnd,2) );
        %             projections(nPortsAcu:nPortsAcuEnd,2)
        %             pause
        Rall(nPortsAcu:nPortsAcuEnd) = R';
        
        % conductance of the model
        G = At*wb(tagsfor)*pb(tagsfor)*cb(tagsfor);
        
        % current source of the model
        Is = At*( Qmet(tagsfor) + wb(tagsfor)*pb(tagsfor)*cb(tagsfor)*Tb(tagsfor) );
        
        % Zhat
        Zhat = Z(1)*Z(2)*Z(3)/(Z(1)*Z(2) + Z(1)*Z(3) + Z(2)*Z(3) + Z(1)*Z(2)*Z(3)*G);
        
        % transmitances
        tau = 2*Zhat./Z;
        
        % scattering matrix. From Vi to Vr. This is a process that
        % occurs just into the node. It does not depend from
        % anything outside the node. This is a temporarly variable
        S( nPortsAcu:nPortsAcuEnd, nPortsAcu:nPortsAcuEnd ) = ...
            [ ( tau(1) - 1 ),     tau(2),             tau(3); ...
            tau(1),         ( tau(2) - 1 ),       tau(3); ...
            tau(1),           tau(2),           ( tau(3) - 1 )];
        
        
        % effect of external sources on Vr calculation. This is a
        % temporary variable.
        ZIS( nPortsAcu:nPortsAcuEnd) = [1; 1; 1]*Zhat*Is;
        
        % These matrixes are used when we want to calculate the
        % temperature at node's center.
        %             if CalculateTc == 1
        TM( nNodeAcu, nPortsAcu:nPortsAcuEnd ) = tau;
        TE( nNodeAcu ) = ZIS(nPortsAcu);
        %             end
        
        % if we are not solving for steady-state, we need to calculate
        % the initial value for the incidente voltage
        %             if (SteadyState ~= 1)
        Vi0( nPortsAcu:nPortsAcuEnd) = ( Ti(tagsfor) - ZIS(nPortsAcu) )/sum(tau);
        %             end
        
        
        
        
        
        % here we implement the intersection point
        % The vertex that represents the line must be always lower than
        % the vertex that represents the column
        for convar = 0:2
            switch convar
                case 0
                    if vertexn2 > vertexn3
                        vertext1 = vertexn3;
                        vertext2 = vertexn2;
                    else
                        vertext1 = vertexn2;
                        vertext2 = vertexn3;
                        
                        % here we correct the projections of lL. The
                        % number of the vertex2 is small than the
                        % number of the vertex3
                        projections(nPortsAcu,1) = - projections(nPortsAcu,1);
                    end
                    
                    
                    
                    
                    
                case 1
                    if vertexn1 > vertexn3
                        vertext1 = vertexn3;
                        vertext2 = vertexn1;
                    else
                        vertext1 = vertexn1;
                        vertext2 = vertexn3;
                        
                        % here we corret the projections of lL. The
                        % number of the vertex1 is small than the
                        % number of the vertex3
                        projections(nPortsAcu + 1,1) = - projections(nPortsAcu + 1,1);
                    end
                    
                    
                    
                case 2
                    if vertexn1 > vertexn2
                        vertext1 = vertexn2;
                        vertext2 = vertexn1;
                    else
                        vertext1 = vertexn1;
                        vertext2 = vertexn2;
                        
                        % here we corret the projections of lL. The
                        % number of the vertex1 is small than the
                        % number of the vertex2
                        projections(nPortsAcuEnd,1) = - projections(nPortsAcuEnd,1);
                    end
                    
                    
            end
            
            
            % Is there any intersection point allocated here?
            IPt = IPm(vertext1, vertext2); % intersection point being tested
            if IPt ~= 0 % yes
                IPnPorts(IPt,2) = nPortsAcu + convar; % here we allocate
                % the number of the port in that intersection point
                
            else
                IPm(vertext1, vertext2) = IP; % call this the number of the variable IP
                
                IPnPorts(IP,1) = nPortsAcu + convar; % here we allocate
                % the number of the port in that intersection point
                IP = IP + 1; % increase the intersection point by 1
            end
        end
        
        
        
        
        % now we increase nNodeAcu by 1; nPortsAcu by 3 (for 3 ports in
        % the triangle model). nPortsAcuEnd by two (for 3 ports in the
        % triangle model).
        nNodeAcu = nNodeAcu + 1; nPortsAcu = nPortsAcu + 3; nPortsAcuEnd = nPortsAcu + 2;
        
        
        % this triangle represents a boundary condition
    elseif medium == 0
        
        % tagsfor is the number of the medium that has the parameters
        % we want.
        
        % Element's vertex
        vertexn1 = nEm2(varE2+1,2); % number of vertex 1
        vertexn2 = nEm2(varE2+1,3); % number of vertex 2
        vertexn3 = nEm2(varE2+1,4); % number of vertex 3
        
        % Gmsh gives the values in mm. Here we work in m. Then, we need
        % to multiply the values of the vertex's position by 1e-3.
        vertex1 = vertexmat(vertexn1,:)*1e-3; % vertex 1 (x, y, z) in mm
        vertex2 = vertexmat(vertexn2,:)*1e-3; % vertex 2 (x, y, z) in mm
        vertex3 = vertexmat(vertexn3,:)*1e-3; % vertex 3 (x, y, z) in mm
        
        % face 1
        deltaX(1) = vertex2(1) - vertex3(1);
        deltaY(1) = vertex2(2) - vertex3(2);
        deltaZ(1) = vertex2(3) - vertex3(3);
        % face 2
        deltaX(2) = vertex1(1) - vertex3(1);
        deltaY(2) = vertex1(2) - vertex3(2);
        deltaZ(2) = vertex1(3) - vertex3(3);
%         % face 3
%         deltaX(3) = vertex1(1) - vertex2(1);
%         deltaY(3) = vertex1(2) - vertex2(2);
%         deltaZ(3) = vertex1(3) - vertex2(3);
        
        % Triangle's area.
        At = sqrt(  ( deltaY(1)*deltaZ(2) - deltaY(2)*deltaZ(1) )^2 + ...
            ( deltaX(1)*deltaZ(2) - deltaX(2)*deltaZ(1) )^2 + ...
            ( deltaX(1)*deltaY(2) - deltaX(2)*deltaY(1) )^2   )/2;
        
        
        
        switch boundarytype(tagsfor)
            % case 1: constant temperature
            case 1
                BCtype = -1;
                BCv1 = boundaryvalues(1,tagsfor); % temperature
                BCv2 = 0;
                
                
                % case 2: constant heat flux.
            case 2
                BCtype = -2;
                BCv1 = boundaryvalues(1,tagsfor)*At; % heat flux
                BCv2 = 0;
                
                
                % case 3: Newton's cooling law.
            case 3
                BCtype = -3;
                BCv1 = boundaryvalues(1,tagsfor); % temperature
                BCv2 = boundaryvalues(2,tagsfor)*At; % heat transfer coefficient
        end
        
        
        
        
        vertex = sort([vertexn1 vertexn2 vertexn3]);
        
        % Is there any intersection point allocated here?
        IPt = IPm(vertex(1), vertex(2) + (vertex(3) - 1)*nN); % intersection point being tested
        if IPt ~= 0 % yes
            IPnPorts(IPt,3:5) = [BCtype, BCv1, BCv2];
            % BCtype: tells what boundary type it is
            % BCv1: gives the first pertinent value for this BCtype
            % BCv2: gives the second pertinent value for this BCtype
            
        else
            IPm(vertex(1),vertex(2) + (vertex(3) - 1)*nN) = IP; % call this the number of the variable IP
            IPnPorts(IP,3:5) = [BCtype, BCv1, BCv2];
            
            IP = IP + 1; % increase the intersection point by 1
        end
    end
end

% line nomenclature
%
%   vertex 1  \
%              \
%               \    face
%                \
%                 \  vertex 2

% lines can be boundary conditions and mediums. The medium version was not
% implemented

for varnE1 = 0:(size(nEm1,1)-1)
    tag_phy = nEm1(varnE1+1,1); % getting the number of the physical tag.
    % the physical tag is used for defining mediums and boundary conditions
    
    medium = -1; % if medium = -1, the element was not assigned to be medium neither boundary.
    % if medium = 0; the element was assigned to be boundary. If medium =
    % 1; the element was assigned to be medium
    
    % verifying if this element is medium
    for tagsfor = 1:size(mediumtags,2)
        if tag_phy == mediumtags(tagsfor) % this was not implemented yet
            medium = 1; % flag so that we won't verify if this tag is for a boundary
        end
    end
    
    % verifying if this element is boundary
    if medium ~= 1
        for tagsfor = 1:size(boundarytags,2)
            if tag_phy == boundarytags(tagsfor) % yes, it is a boundary condition
                % Node's vertex
                vertexn1 = nEm1(varnE1+1,2); % number of node 1
                vertexn2 = nEm1(varnE1+1,3); % number of node 2
                
                
                % here we implement the intersection point
                % The vertex that represents the line must be always lower than
                % the vertex that represents the column
                if vertexn1 > vertexn2
                    vertext1 = vertexn2;
                    vertext2 = vertexn1;
                else
                    vertext1 = vertexn1;
                    vertext2 = vertexn2;
                end
                
                % now that we know what boundary number is into the
                % boundarytags variable, we will discover if it is constant
                % temperature or heat flux or others
                switch boundarytype(tagsfor)
                    % case 1: constant temperature
                    case 1
                        BCtype = -1;
                        BCv1 = boundaryvalues(1,tagsfor); % temperature
                        BCv2 = 0;
                        
                        
                        
                        % case 2: constant heat flux.
                    case 2
                        % we will multiply BCv1 by the length of the
                        % triangular face
                        L = sqrt( (vertexmat(vertexn1,1) - vertexmat(vertexn2,1))^2 + ...
                            (vertexmat(vertexn1,2) - vertexmat(vertexn2,2))^2 + ...
                            (vertexmat(vertexn1,3) - vertexmat(vertexn2,3))^2);
                        BCtype = -2;
                        BCv1 = boundaryvalues(1,tagsfor)*L*1e-3; % heat flux
                        BCv2 = 0;
                        
                        
                        % case 3: Newton's cooling law.
                    case 3
                        % we will multiply BCv2 by the length of the
                        % triangular face
                        L = sqrt( (vertexmat(vertexn1,1) - vertexmat(vertexn2,1))^2 + ...
                            (vertexmat(vertexn1,2) - vertexmat(vertexn2,2))^2 + ...
                            (vertexmat(vertexn1,3) - vertexmat(vertexn2,3))^2);
                        BCtype = -3;
                        BCv1 = boundaryvalues(1,tagsfor); % temperature
                        BCv2 = boundaryvalues(2,tagsfor)*L*1e-3; % heat transfer coefficient
                end
                
                
                
                
                % Is there any intersection point allocated here?
                IPt = IPm(vertext1, vertext2); % intersection point being tested
                if IPt ~= 0 % yes
                    IPnPorts(IPt,3:5) = [BCtype, BCv1, BCv2];
                    % BCtype: tells what boundary type it is
                    % BCv1: gives the first pertinent value for this BCtype
                    % BCv2: gives the second pertinent value for this BCtype
                    
                else
                    IPm(vertext1, vertext2) = IP; % call this the number of the variable IP
                    IPnPorts(IP,3:5) = [BCtype, BCv1, BCv2];
                    
                    IP = IP + 1; % increase the intersection point by 1
                end
                
                break % get out the for loop and go to the next node
            end
        end
    end
end



% Now that we have calculated the Scattering matrix, the external source matrix,
% and related the nodes between each other, we will proceed to calculate
% the Connection matrix and the boundary condition matrix.

% here we know exactly how many intersections we have, this number is IP - 1.
% Then, we will create a matrix that will convert the values of the
% temperatures on the elements in to the values of the temperatures between
% elements.
% Te2Tc = sparse(IP-1,szt);
% BmatTe2Tc = sparse(IP-1,1); % effect of boundaries in the calculation of heat fluxes.
posimatbtw = zeros(IP-1,3); % this contains x, y, z for the between points.

% in addition, we will create the matrix for calculating the temperatures
% and heat fluxes in the ports
beta = sparse(IP-1,szmt);
TB = sparse(IP-1,1);

% freeing some memory space
clear IPm
for nvZeq = 1:(IP-1)
    
    % the procedure for caculating the connection matrix depends on if the
    % intersection point has a boundary condition
    switch IPnPorts(nvZeq,3)
        
        % this is the case without boundary conditions
        case 0
            
            P1 = IPnPorts(nvZeq,1); % number of port 1
            P2 = IPnPorts(nvZeq,2); % number of port 2
            
            
            if P2 == 0 % this means that this intersection point
                % just have one port. When that happens, we treat it as an
                % insulated boundary condition. Namely qflux = 0.
                C(P1,P1) = 1;
                % position of the intersection point
                posimatbtw(nvZeq,:) = posimatports(P1,:);
                
                beta(nvZeq,P1) = 2;
                
            else % this intersection point has 2 ports
                Zt = ( Rall(P2) + Zall(P2) + Rall(P1) + Zall(P1) );
                C(P1,P1) = ( Rall(P2) + Zall(P2) + Rall(P1) - Zall(P1) )/Zt;
                C(P2,P2) = ( Rall(P1) + Zall(P1) + Rall(P2) - Zall(P2) )/Zt;
                C(P2,P1) = 2*Zall(P2)/Zt;% from port 1 to port 2
                C(P1,P2) = 2*Zall(P1)/Zt;% from port 2 to port 1
                
                % position of the intersection point
                posimatbtw(nvZeq,:) = posimatports(P1,:);
                
                beta(nvZeq,P1) = 2 - 2*(Rall(P1) + Zall(P1))/Zt;
                beta(nvZeq,P2) = 2 - 2*(Rall(P2) + Zall(P2))/Zt;
                
            end
            
            % this is the case with temperature as boundary condition
        case -1
            
            if IPnPorts(nvZeq,2) == 0
                P1 = IPnPorts(nvZeq,1); % number of port 1
                C(P1,P1) = ( Rall(P1) - Zall(P1) )/( Rall(P1) + Zall(P1) );
                E(P1,1) = IPnPorts(nvZeq,4)*Zall(P1)/( Rall(P1) + Zall(P1) );
                
                % position of the intersection point
                posimatbtw(nvZeq,:) = posimatports(P1,:);
                
                beta(nvZeq,P1) = 0;
                TB(nvZeq) = IPnPorts(nvZeq,4);
            else
                error('Temperature as a boundary conditions between two ports was not implemented yet.')
            end
            
            
            
            
            % this is the case with heat flux as boundary condition
        case -2
            if IPnPorts(nvZeq,2) == 0
                IPnPorts(nvZeq,:);
                P1 = IPnPorts(nvZeq,1);
                C(P1,P1) = 1;
                E(P1,1) =  Zall(P1)*IPnPorts(nvZeq,4);
                
                % position of the intersection point
                posimatbtw(nvZeq,:) = posimatports(P1,:);
                
                % position of the intersection point
                posimatbtw(nvZeq,:) = posimatports(P1,:);
                
                beta(nvZeq,P1) = 2;
                TB(nvZeq) = (Rall(P1) + Zall(P1))*IPnPorts(nvZeq,4);
                
                
            else
                error('Heat flux as a boundary conditions between two ports was not implemented yet.')
            end
            
            % this is the case with Newton's cooling law as boundary
            % condition (not implemented yet)
        case -3
            if IPnPorts(nvZeq,2) == 0
                P1 = IPnPorts(nvZeq,1);
                Rc = 1/( IPnPorts(nvZeq,5) );
                C(P1,P1) = ( Rc + Rall(P1) - Zall(P1) )...
                    /( Rc + Rall(P1) + Zall(P1) );
                E(P1,1) =  Zall(P1)/( Rc + Rall(P1) + Zall(P1) )*IPnPorts(nvZeq,4);
                
                % position of the intersection point
                posimatbtw(nvZeq,:) = posimatports(P1,:);
                
                beta(nvZeq,P1) = 2*Rc/( Rc + Rall(P1) + Zall(P1) );
                TB(nvZeq) = ( Rall(P1) + Zall(P1) )/( Rc + Rall(P1) + Zall(P1) )*IPnPorts(nvZeq,4);
                
                
            else
                error('Newtons cooling law as a boundary conditions was not implemented yet.')
            end
            
            
    end
end

% final matrixes
M = C*S;
E = C*ZIS + sparse(E); % E = B previous to this operation


TM = [TM; beta*S]; % TM = tau preivous to this operation
TE = [TE; TB + beta*ZIS]; % TE = ZIS1 previous to this operation
TM = sparse(TM);
TE = sparse(TE);

positions = [posimat; posimatbtw];


% now we will prepare the matrix for the heat fluxes
% qLTM = zeros(IP-1,szmt);
% qLTE = zeros(IP-1,1);
% etasM = zeros(IP-1,2);

qLTM = sparse(IP-1,szmt);
qLTE = sparse(IP-1,1);
etasM = sparse(IP-1,2);

projec = zeros(IP-1,3);

for nvZeq = 1:(IP-1)
    P1 = IPnPorts(nvZeq,1); % number of port 1
    P2 = IPnPorts(nvZeq,2); % number of port 2
    
    tempM = ( TM( szt + nvZeq,:) - ... % the effect of Tb
        TM( nPort2nNode(P1),: ) )/... % the effect of Tc
        ( projections(P1,3)*Rall(P1) ); % adjusting with L*R or A*R
    
    tempE = ( TE( szt + nvZeq) - ... % the effect of Tb
        TE( nPort2nNode(P1) ) )/... % the effect of Tc
        ( projections(P1,3)*Rall(P1) ); % adjusting with L*R or A*R
    
    qLTM(nvZeq,:) = tempM; % angle for x
    
    qLTE(nvZeq) = tempE; % angle for x
    projec(nvZeq,:) = [projections(P1,4) projections(P1,5) projections(P1,6)];
    etasM(nvZeq,1) = etas(P1);
    if P2 ~= 0
        etasM(nvZeq,2) = etas(P2);
    end
    
end

% making the matrixes sparse
qLTM = sparse(qLTM);
qLTE = sparse(qLTE);
