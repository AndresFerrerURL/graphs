#version 330

// Uniforms
uniform sampler2D u_texture;  // Textura aplicada al objeto
uniform vec3 u_light_dir;     // Direcci�n de la luz
uniform vec3 u_color;         // Color base del objeto (opcional)
uniform vec3 u_ambient;       // Luz ambiental
uniform vec3 u_diffuse;       // Luz difusa
uniform vec3 u_specular;      // Luz especular
uniform float u_shininess;    // Brillo del material
uniform vec3 u_cam_pos;       // Posici�n de la c�mara (para Blinn-Phong)

// Inputs del vertex shader
in vec2 v_uv;                 // Coordenadas UV para la textura
in vec3 v_normal;             // Normal interpolada del fragmento

// Salida final del color del fragmento
out vec4 fragColor;

void main() {
    // Normalizaci�n de vectores
    vec3 N = normalize(v_normal);         // Normal del fragmento
    vec3 L = normalize(u_light_dir);      // Direcci�n de la luz normalizada
    vec3 E = normalize(u_cam_pos - gl_FragCoord.xyz); // Direcci�n hacia la c�mara
    vec3 H = normalize(L + E);            // Half-vector (Blinn-Phong)

    // C�lculo de la iluminaci�n difusa (Lambert)
    float NdotL = max(dot(N, L), 0.0);    // Producto escalar entre normal y luz

    // C�lculo de la iluminaci�n especular (Blinn-Phong)
    float NdotH = max(dot(N, H), 0.0);    // Producto escalar entre la normal y el half-vector
    float specular = pow(NdotH, u_shininess); // Especular con el brillo

    // Componentes de la iluminaci�n
    vec3 ambient_color = u_ambient;
    vec3 diffuse_color = u_diffuse * NdotL; // Iluminaci�n difusa
    vec3 specular_color = u_specular * specular; // Iluminaci�n especular

    // Color final (mezcla de difusa, especular y ambiental)
    vec3 texture_color = texture(u_texture, v_uv).rgb;
    vec3 final_color = texture_color * (ambient_color + diffuse_color + specular_color);

    // Salida del fragment shader
    fragColor = vec4(final_color, 1.0); // Color con alfa fijo a 1.0
}
