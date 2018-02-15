#version 130

uniform vec4 PickingColor;
 
out vec4 outcolor;
 
void main()
{
    outcolor = PickingColor;
}