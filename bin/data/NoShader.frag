varying vec2        texcoord0;
uniform sampler2D   tex0;

void main()
{
    gl_FragColor = texture2D(tex0, texcoord0);
}
