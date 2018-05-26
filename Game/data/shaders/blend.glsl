uniform sampler2D texture;
uniform sampler2D lightmap;

void main() {
    vec4 t0 = texture2D(texture, gl_TexCoord[0].st);
    vec4 t1 = texture2D(lightmap, gl_TexCoord[0].st);
	vec4 tmp;
	tmp.a=1.0;
	tmp.r=t1.r*t0.r;
	tmp.g=t1.g*t0.g;
	tmp.b=t1.b*t0.b;
    gl_FragColor = tmp;

}
