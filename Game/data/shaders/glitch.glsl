

uniform sampler2D img;
uniform float center;
uniform vec3 offsets;
uniform vec3 add;

void main()
{
	vec2 pos = gl_TexCoord[0].xy;
	vec4 c=vec4(0.,0.,0.,1.);
	float k;
	if(pos.y>center){ 
		k=(0.7/pow(max(abs(pos.y-center)*10.0, 1.0), 4.0) + 0.3);
		c.r+=texture2D(img,pos+(offsets.r*k)).r+add.r;
		c.g+=texture2D(img,pos+(offsets.g*k)).g+add.g;
		c.b+=texture2D(img,pos+(offsets.b*k)).b+add.b;
	}
	else{ 
		k=-(0.7/pow(max(abs(pos.y-center)*10.0, 1.0), 4.0) + 0.3);
		c.r+=texture2D(img,pos+(offsets.r*k)).r+add.r;
		c.g+=texture2D(img,pos+(offsets.g*k)).g+add.g;
		c.b+=texture2D(img,pos+(offsets.b*k)).b+add.b;
	}

	gl_FragColor = c;
}

