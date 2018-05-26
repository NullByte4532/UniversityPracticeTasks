uniform sampler2D texture ;
uniform sampler2D shadow;
uniform vec3 light;
uniform vec3 dir;
uniform vec3 lcolor;
uniform float decay;
void main() {
	vec2 pos = gl_TexCoord[0].xy ; 
	float sh = texture2D(shadow,pos).r;
	if(sh==0.0){
		gl_FragColor=texture2D(texture,pos);
		return;
	}

	float distance = sqrt(pow(gl_FragCoord.x - light.x, 2.0) + pow(gl_FragCoord.y - light.y, 2.0));
	vec2 ray;
	ray.x=(gl_FragCoord.x-light.x)/distance;
	ray.y=(gl_FragCoord.y-light.y)/distance;
	if (floor(light.x) == floor(gl_FragCoord.x) && floor(light.y) == floor (gl_FragCoord.y)) 
        distance = 0.1;
 	//if(distance<2.1)distance=2.1;
	float proj;
	proj=ray.x*dir.x+ray.y*dir.y;
	proj=1.0+(dir.z-proj)*20.0;
	if(proj<1.0) proj=1.0;
	//float min_dis = pow((2.0*light.z*255.0/(max(max(lcolor.x,lcolor.y),lcolor.z)*proj)-1.0), 1.0/decay);
	//if(distance<min_dis) distance=min_dis;
	vec4 pisos;
	pisos.r=lcolor.x*sh;
	pisos.g=lcolor.y*sh;
	pisos.b=lcolor.z*sh;
	pisos.a=1.0;
	
	vec4 tmp;
	tmp.a=1.0;
	tmp.r=min(255.0, texture2D(texture,pos).r+pisos.r*((light.z*2.0/(1.0+pow(distance, decay)))/proj));
	tmp.g=min(255.0, texture2D(texture,pos).g+pisos.g*((light.z*2.0/(1.0+pow(distance, decay)))/proj));
	tmp.b=min(255.0, texture2D(texture,pos).b+pisos.b*((light.z*2.0/(1.0+pow(distance, decay)))/proj));
	gl_FragColor = tmp;
   // gl_FragColor = mix(texture2D(texture,pos), pisos, (light.z*2.0/(1.0+pow(distance, decay)))/proj); 

}
