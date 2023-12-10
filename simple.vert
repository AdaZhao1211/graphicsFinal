//  Identity vertex shader
// Constant Color Shader
// glsl 1.2 version
#version 120

void main()
{
   //  Use color unchanged
   gl_FrontColor = gl_Color;
   // glColor Attribute type
   // gl Front Color Varying type because it is gonna be passed to frag shader

   //  Set vertex coordinates
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
   //  gl_vertex: attribute
   // gl position: varying

   // *model view matrix *projection matrix
   // gl_ModelViewProjectionMatrix: uniform
}
