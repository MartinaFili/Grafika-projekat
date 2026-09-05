# Rooftop

198/2020 - Martina Filipovic  
The scene depicts a building rooftop at night.
Rooftop props (ventilation, an access hatch) sit at the center of the roof.
Pressing SPACE causes a flying saucer to appear above the roof;
after a short delay it descends toward the roof,
casting a spotlight whose cone narrows and intensifies as it gets closer.
After landing, the saucer disappears following another short pause.
The scene is additionally lit by moonlight (a directional light)
whose intensity can be adjusted through a GUI slider.

## Controls

SPACE -> triggers the saucer's descent (or restarts it after it has disappeared)  
R -> opens/closes the GUI panel for adjusting moonlight intensity  
Mouse -> rotates the camera view  
ESC -> exits the application

## Features

### Fundamental:

[x] Model with lighting
[x] Two types of lighting with customizable colors and movement through GUI or ACTIONS
[x] {SPACE press} --- AFTER 2 SECONDS ---Triggers---> {saucer starts descending, spotlight intensifies and narrows} --->
AFTER 2 SECONDS---Triggers---> {saucer and light disappear}

### Group A:

[ ] Frame-buffers with post-processing   
[ ] Off-screen Anti-Aliasing  
[ ] Parallax Mapping
[ ] Bloom with the use of HDR

### Group B:

[ ] Deferred Shading  
[ ] Point Shadows  
[ ] SSAO

### Engine improvement:

[x] Floor quad rendering (VAO/VBO for a textured flat surface, added to the OpenGL class — init_floor_quad and
draw_floor_quad)
[x] Texture unit helper (OpenGL::texture_unit — converts a plain index to the corresponding GL_TEXTUREn constant)

## Models:

[Rooftop props] (https://sketchfab.com/3d-models/rooftop-assets-dfd3b70f5831422fb1aee537feb1dc97)   
[Flying saucer] (https://sketchfab.com/3d-models/retro-ufo-a198a969caed46f4b58ff30167339650)

## Textures

[Rooftop floor] (https://ambientcg.com/view?id=GravelSubstance002)

## Skybox

[City rooftop night skybox] (https://sketchfab.com/3d-models/city-rooftop-night-skybox-cce0e8aaa10f45ccb234e1aa2a3d5753)
