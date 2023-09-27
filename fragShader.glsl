#version 430


out vec4 fragmentColor;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;


void main(void)
{
	fragmentColor = vec4(29.0,34.0,105.0, 1.0); 
}
