# tlmbht
Transmission-line Modeling Method applied to BioHeat Transfer Problems

This is scientific code used to solve the bioheat equation (BHE; Pennes, 1948) in 2D and 3D problems. The solver is the TLM method and more information is provided below. The language is MATLAB and it does not have graphical interface (yet).

I hope you will find it useful for learning/use the Transmission-line Modeling (TLM) method applied to solve the Bioheat transfer problems. The code developed herein is based in two new nodes that I recently developed for solving the bioheat equation. The nodes are triangular for 2-D problems (Milan and Gebremedhin, 2016a) and tetrahedron for 3-D problems (Milan and Gebremedhin, 2016b). I will provide some theory when the papers get published.

I started developing the TLMBHT in Matlab but I intend to code it in another language. Accompaning the code, follows a code for solving the BHE in 2D and 3D analyticaly for simplified geometries. The geometries are rectangular and cubic. I will discuss them after the papers get published (e-mail me if you need them though).

Please, fell free to use the software for educational and research purposes. You can modify it and your modifications can be (if you want to) included into this project. The limitation is that it cannot be used for profitable purposes (due to Cornell License constrains; please, read the license for more information).

<b>References</b>

Pennes HH. Analysis of tissue and arterial blood temperature in the resting human forearm. J. Appl. Phys. 1:93-122, 1948.

Milan HFM, Gebremedhin KG. Triangular node for transmission-line modeling (TLM) applied to bioheat transfer. Submitted to J Therm Biol, 2016a.

Milan HFM, Gebremedhin KG. Tetrahedral node for transmission-line modeling (TLM) applied to bioheat transfer. Submitted to J Therm Biol, 2016b.
