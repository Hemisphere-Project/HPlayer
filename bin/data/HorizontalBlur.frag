precision highp float;

varying vec2 texcoord0;
uniform sampler2D tex0;
uniform float radius;

const float total = (1. + 8. + 28. + 56.) * 2. + 70.;

void main()
{
	vec2 st = texcoord0.st;

	gl_FragColor += (1. / total) * texture2D(tex0, st - radius * vec2(4. / 4., 0.));
	gl_FragColor += (8. / total)  * texture2D(tex0, st - radius * vec2(3. / 4., 0.));
	gl_FragColor += (28. / total)  * texture2D(tex0, st - radius * vec2(2. / 4., 0.));
	gl_FragColor += (56. / total)  * texture2D(tex0, st - radius * vec2(1. / 4., 0.));

	gl_FragColor +=  (70. / total) * texture2D(tex0, st);

	gl_FragColor += (1. / total) * texture2D(tex0, st + radius * vec2(4. / 4., 0.));
	gl_FragColor += (8. / total)  * texture2D(tex0, st + radius * vec2(3. / 4., 0.));
	gl_FragColor += (28. / total)  * texture2D(tex0, st + radius * vec2(2. / 4., 0.));
	gl_FragColor += (56. / total)  * texture2D(tex0, st + radius * vec2(1. / 4., 0.));
}
