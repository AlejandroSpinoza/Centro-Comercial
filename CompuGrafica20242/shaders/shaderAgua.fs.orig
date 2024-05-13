#version 330 core

uniform vec3 waterColor;
uniform float time;  // Pasa el tiempo desde la aplicación al shader

out vec4 FragColor;

void main()
{
    // Ajusta la frecuencia, amplitud y desfase según tus necesidades
    float frequency = 1.0;
    float amplitude = 0.1;
    float phaseOffset = 0.5;  // Ajusta el desfase aquí

    // Calcula las coordenadas del fragmento en el espacio del mundo
    vec3 worldPos = gl_FragCoord.xyz;

    // Simula olas utilizando funciones trigonométricas con desfase
    float wave = amplitude * sin(worldPos.x * frequency + time + phaseOffset);

    // Combina el color del agua con el efecto de las olas
    vec3 finalColor = waterColor + vec3(wave);

    // Asigna el color resultante a FragColor
    FragColor = vec4(finalColor, 1.0);
}
