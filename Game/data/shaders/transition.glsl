
uniform sampler2D img1;
uniform sampler2D img2;
uniform float loffsets[16];
uniform vec3 coffsets;
void main()
{
	
	vec2 pos = gl_TexCoord[0].xy;
	int line = int(16.0*pos.y);
	vec4 col = vec4(0.,0.,0.,1.);	
	float tmp;
	tmp = coffsets.x*loffsets[line]+pos.x;
	if(tmp<0.0){
		col.r=texture2D(img2, vec2(1.0+tmp, pos.y)).r;
	}else if(tmp>=1.0){
		col.r=texture2D(img2, vec2(tmp-1.0, pos.y)).r;

	}else{
		col.r=texture2D(img1, vec2(tmp, pos.y)).r;
	}
	tmp = coffsets.y*loffsets[line]+pos.x;
	if(tmp<0.0){
		col.g=texture2D(img2, vec2(1.0+tmp, pos.y)).g;
	}else if(tmp>=1.0){
		col.g=texture2D(img2, vec2(tmp-1.0, pos.y)).g;

	}else{
		col.g=texture2D(img1, vec2(tmp, pos.y)).g;
	}
	tmp = coffsets.z*loffsets[line]+pos.x;
	if(tmp<0.0){
		col.b=texture2D(img2, vec2(1.0+tmp, pos.y)).b;
	}else if(tmp>=1.0){
		col.b=texture2D(img2, vec2(tmp-1.0, pos.y)).b;

	}else{
		col.b=texture2D(img1, vec2(tmp, pos.y)).b;
	}
	gl_FragColor = col;

}
