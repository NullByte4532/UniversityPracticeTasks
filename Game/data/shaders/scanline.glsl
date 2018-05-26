#version 130

uniform sampler2D texture;
uniform float amount = 0.3;
uniform float thickness = 2.0;
uniform float spacing = 2.0;

void main()
{
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy) * gl_Color;
	if (mod(gl_FragCoord.y, round(thickness + spacing)) < round(spacing))
		//pixel = vec4(pixel.rgb * (1.0 - amount), pixel.a);
		pixel=mix(pixel, vec4(0.0,0.0,0.0,1.0), amount);
	gl_FragColor = pixel;
}
