function [errormsg, nN, vertexmat, ntags, nEm1, ...
    nEm2, nEm3, nEm4, nEm5, nEm6, nEm7, nEm15] = openmsh()
% This file is part of TLMBHT. See details of this function below.
% Version 0.1. Copyright © 2015 to 2016 by Cornell University.
% Written by Hugo Fernando Maia Milan (hugofernando@gmail.com).
% All Rights Reserved. Refer to the license file for details.
%
% Acknowledgment: Brazilian National Counsel of Technological and Scientific
% Development (Proc. 203312/2014-7) for scholarship to HFMM
%
%
% This function open a window and asks the user to select the .msh file
% that he wants to read. The output are data that Tlmbht interprets.

%
% textscan in octave gives the line reading
% textscan in MATLAB gives only one variable
%

% declaring variables
nN = 0; vertexmat = 0; tagmatrix = 0; nE = 0; nE1 = 0; nEm1 = 0;
nE2 = 0; nEm2 = 0; nE3 = 0; nEm3 = 0; nE4 = 0; nEm4 = 0; nE5 = 0; nEm5 = 0;
nE6 = 0; nEm6 = 0; nE7 = 0; nEm7 = 0; nE15 = 0; nEm15 = 0;

[filename, pathname] = uigetfile('.msh','Select the .msh file');
% ID# for the file
file_id = fopen([pathname,filename]);
% Buffer variable for sending error messages.
errormsg = '';
reading = textscan(file_id, '%s', 1);
% Testing if the first line is the $MeshFormat
if ~strcmp('$MeshFormat',reading{1,1})
    errormsg = 'This is not a .msh file. Please, verify your file.';
    fclose('all');
    return;
end
reading = textscan(file_id, '%s', 1);
% Testing if the mesh version 2.2
if ~strcmp('2.2',reading{1,1}{1,1})
    errormsg = 'This mesh is not version 2.2. Only works for .msh version 2.2.';
    fclose('all');
    return;
end
% Testing if mesh is an ASCII file type
if ~strcmp('0',reading{1,1}{2,1})
    errormsg = 'This file did not come from an ASCII file. Only reads ASCII meshes.';
    fclose('all');
    return;
end
reading = textscan(file_id, '%s',1);
% Verifying if the stadandard is ok
if ~strcmp('$EndMeshFormat',reading{1,1})
    errormsg = 'Sorry. Unknown error reading the file. We did not find the section $EndMeshFormat.';
    fclose('all');
    return;
end
reading = textscan(file_id, '%s',1);
% Read until you get to the $Nodes section
while ~strcmp('$Nodes',reading{1,1})
    reading = textscan(file_id, '%s',1);
    
    % We did not find the $Nodes section. For standard, the
    % $Nodes section must come first than the $Elements
    % section
    if isempty(reading{1,1})
        errormsg = 'Sorry. Unknown error reading the file. We did not find the section $Nodes.';
        fclose('all');
        return;
    end
end

% Processing the Nodes' information
if ~strcmp('$Nodes',reading{1,1})
    errormsg = 'Sorry. Unknown error reading the file. We did not find the section $Nodes.';
    fclose('all');
    return;
end
nN = textscan(file_id, '%s',1); % this is the number of nodes in the mesh. The vertices in TLM
nN = str2double(nN{1,1}{1});

% Creating the matrix that will containg the
% information from the nodes. The line correspond to
% the nodes number and the columns to (x,y,z).
vertexmat = zeros(nN,3);

for readnode = 1:nN
    nodei = textscan(file_id, '%s',1);
    % node's number = x,y,z
    vertexmat(str2double(nodei{1,1}{1}),:) = [str2double(nodei{1,1}{2}) ...
        str2double(nodei{1,1}{3}) str2double(nodei{1,1}{4})];
end

% checking if we have reach the end of the node's
% information
reading = textscan(file_id, '%s',1);
if ~strcmp('$EndNodes',reading{1,1})
    errormsg = 'Sorry. Unknown error reading the file. We did not find the section $EndNodes.';
    fclose('all');
    return;
end
reading = textscan(file_id, '%s',1);
% Read until you get to the $Elements section
while ~strcmp('$Elements',reading{1,1})
    reading = textscan(file_id, '%s',1);
    
    % We did not find the $Elements section. For standard, the
    % $Elements section must come after the $Nodes
    % section
    if isempty(reading{1,1})
        errormsg = 'Sorry. Unknown error reading the file. We did not find the section $Elements.';
        fclose('all');
        return;
    end
end

% Processing the Elements' information
if ~strcmp('$Elements',reading{1,1})
    errormsg = 'Sorry. Unknown error reading the file. We did not find the section $Elements.';
    fclose('all');
    return;
end
nE = textscan(file_id, '%s',1); % this is the number of elements
nE = str2double(nE{1,1}{1});

% We allocate the number of possible elements.
% See elemtype variable for number's meaning.
nE1 = 0; nE2 = 0; nE3 = 0; nE4 = 0; nE5 = 0;
nE6 = 0; nE7 = 0; nE15 = 0;


% matrix that will allocate the nodes each element number contains
elements = zeros(nE,10);



% this variable will contain the quantity of
% elements that has a particular tag number.
% the lines represent the different tag
% numbers. Column 1 represent the number of the
% tag and column 2-9 the quantity of a specific
% element type in that tag number.
% 2: 2-node line. 3: 3-node triangle.
% 4: 4-node quadrangle. 5: 4-node tetrahedron.
% 6: 8-node hexahedron. 7: 6-node prism.
% 8: 5-node pyramid. 9: 1-node point.
ntags = zeros(1,9);

% reading the information from the elements
for readelem = 1:nE
    % This is the number of the element
    reading = textscan(file_id, '%s',1);
    readi = str2double(reading{1,1}{1});
    
    % This is the element type. For gmsh, some
    % numbers are: 1 (2-node line), 2 (3-node
    % triangle), 3 (4-node quadrangle), 4
    % (4-node tetrahedron), 5 (8-node
    % hexahedron), 6 (6-node prism), 7 (5-node
    % pyramid), 15 (1-node point). See Gmsh's
    % manual for more information.
    elemtype = str2double(reading{1,1}{2});
    
    % This is the number of tags for the
    % element_i. By default, the 1st tag is the
    % number of the physical entity to which
    % the element belongs, the 2nd tag is the
    % number of elementary geometrical entity
    % to which the element belongs, the 3rd is
    % the number of mesh partitions to which
    % the element belongs, followed by the
    % partition ids (4th number). Negative
    % partition ids indicate ghost cells.
    tagn = str2double(reading{1,1}{3});
    
    % Reading the tags information
    tagntemp = str2double(reading{1,1}{4});
    
    
    % now we compute the number of tag
    readtag = 1; % flag to identify if the number
    % already exist in the ntags matrix. It
    % will be 1 if it exist and 0 if not. In
    % case it is 0, a new line for ntags will
    % be created.
    % has the tag number already read?
    for snt = 1:size(ntags,1)
        if tagntemp == ntags(snt,1)
            if elemtype == 15
                ntags(snt,9) = ntags(snt,9) + 1;
            else
                ntags(snt,elemtype + 1) = ntags(snt,elemtype + 1) + 1;
            end
            readtag = 0;
            break
        end
    end
    
    % the tag number has not read yet. Then
    % creat a new line for the ntags matrix
    if readtag
        ntags(end+1,:) = zeros(1,9);
        ntags(end,1) = tagntemp;
        if elemtype == 15
            ntags(end,9) = ntags(end,9) + 1;
        else
            ntags(end,elemtype + 1) = ntags(end,elemtype + 1) + 1;
        end
    end
    
    % now we allocate the element according to
    % its type. Note that in Matlab only one
    % case  from the switch-case structure is executed.
    % It was organized for the most common elements
    % first.
    switch (elemtype)
        
        case (4) % 4-node tetrahedron
            nE4 = nE4 + 1;            
            elements(readelem,1:6) = [4 tagntemp str2double(reading{1,1}{4 + tagn}) ...
                str2double(reading{1,1}{5 + tagn}) ...
                str2double(reading{1,1}{6 + tagn}) ...
                str2double(reading{1,1}{7 + tagn})];
            
        case (2) % 3-node triangle
            nE2 = nE2 + 1;
            
            elements(readelem,1:5) = [2 tagntemp str2double(reading{1,1}{4 + tagn}) ...
                str2double(reading{1,1}{5 + tagn}) ...
                str2double(reading{1,1}{6 + tagn})];
            
        case (1) % 2-node line
            nE1 = nE1 + 1;
            
            elements(readelem,1:4) = [1 tagntemp str2double(reading{1,1}{4 + tagn}) ...
                str2double(reading{1,1}{5 + tagn})];
            
        case (15) % 1-node point
            nE15 = nE15 + 1;
            
            elements(readelem,1:3) = [15 tagntemp str2double(reading{1,1}{4 + tagn})];
            
        case (3) % 4-node quadrangle
            nE3 = nE3 + 1;
            
            elements(readelem,1:6) = [3 tagntemp str2double(reading{1,1}{4 + tagn}) ...
                str2double(reading{1,1}{5 + tagn}) ...
                str2double(reading{1,1}{6 + tagn}) ...
                str2double(reading{1,1}{7 + tagn})];
            
        case (5) % 8-node hexahedron
            nE5 = nE5 + 1;
            
            elements(readelem,:) = [5 tagntemp str2double(reading{1,1}{4 + tagn}) ...
                str2double(reading{1,1}{5 + tagn}) ...
                str2double(reading{1,1}{6 + tagn}) ...
                str2double(reading{1,1}{7 + tagn}) ...
                str2double(reading{1,1}{8 + tagn}) ...
                str2double(reading{1,1}{9 + tagn}) ...
                str2double(reading{1,1}{10 + tagn}) ...
                str2double(reading{1,1}{11 + tagn})];
            
            
            
        case (6) % 6-node prism
            nE6 = nE6 + 1;
            
            elements(readelem,1:8) = [6 tagntemp str2double(reading{1,1}{4 + tagn}) ...
                str2double(reading{1,1}{5 + tagn}) ...
                str2double(reading{1,1}{6 + tagn}) ...
                str2double(reading{1,1}{7 + tagn}) ...
                str2double(reading{1,1}{8 + tagn}) ...
                str2double(reading{1,1}{9 + tagn})];
            
        case (7) % 5-node pyramid
            nE7 = nE7 + 1;
            
            elements(readelem,1:7) = [7 tagntemp str2double(reading{1,1}{4 + tagn}) ...
                str2double(reading{1,1}{5 + tagn}) ...
                str2double(reading{1,1}{6 + tagn}) ...
                str2double(reading{1,1}{7 + tagn}) ...
                str2double(reading{1,1}{8 + tagn})];
            
        otherwise
            errormsg = ['Sorry. Error reading the elements. ' ...
                'Unknown element type number ', num2str(elemtype), '.'];
            fclose('all');
            return;
    end
end

% after we have read for all elements, we will segment for each variable

% We allocate the structure of the matrix for
% each element type. The first column means the
% number of the element. The remaining columns
% mean the nodes that belongs to this element.
% See elemtype variable for number's meaning
nEm1 = zeros(nE1,3);
nEm2 = zeros(nE2,4);
nEm3 = zeros(nE3,5);
nEm4 = zeros(nE4,5);
nEm5 = zeros(nE5,9);
nEm6 = zeros(nE6,7);
nEm7 = zeros(nE7,6);
nEm15 = zeros(nE15,2);

nE1 = 0; nE2 = 0; nE3 = 0; nE4 = 0; nE5 = 0;
nE6 = 0; nE7 = 0; nE15 = 0;
% nE
for readelem = 1:nE
%     elements(readelem,1)
%     pause
    switch (elements(readelem,1))
        case (4) % 4-node tetrahedron
            nE4 = nE4 + 1;
            nEm4(nE4,:) = elements(readelem,2:6);
            
        case (2) % 3-node triangle
            nE2 = nE2 + 1;
            nEm2(nE2,:) = elements(readelem,2:5);
            
        case (1) % 2-node line
            nE1 = nE1 + 1;
            nEm1(nE1,:) = elements(readelem,2:4);
            
        case (15) % 1-node point
            nE15 = nE15 + 1;
            nEm15(nE15,:) = elements(readelem,2:3);
            
        case (3) % 4-node quadrangle
            nE3 = nE3 + 1;
            nEm3(nE3,:) = elements(readelem,2:6);
            
        case (5) % 8-node hexahedron
            nE5 = nE5 + 1;
            nEm5(nE5,:) = elements(readelem,2:10);
            
        case (6) % 6-node prism
            nE6 = nE6 + 1;
            nEm6(nE6,:) = elements(readelem,2:8);
            
        case (7) % 5-node pyramid
            nE7 = nE7 + 1;
            nEm7(nE7,:) = elements(readelem,2:7);
            
        otherwise
            errormsg = ['Sorry. Error reading the elements. ' ...
                'Unknown element type number ', num2str(elemtype), '.'];
            fclose('all');
            return;
    end
end


% checking if we have reach the end of the element's
% information
reading = textscan(file_id, '%s',1);
if ~strcmp('$EndElements',reading{1,1}) && strcmp(errormsg,'')
    errormsg = 'Sorry. Unknown error reading the file. We did not find the section $EndElements.';
    fclose('all');
    return;
end


fclose('all');

