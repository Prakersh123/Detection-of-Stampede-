#version 330 core
out vec4 FragColor;
in vec3 final_col;

void main()
{
    // FragColor = vec4(0.0f,0.0f, 0.0f, 1.0f);
    FragColor= vec4(final_col,1.0f);
}

