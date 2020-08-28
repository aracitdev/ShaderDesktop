# ShaderDesktop

ShaderDesktop is a program which allows you to use fragment shaders as your desktop background.
This is actually can be very intensive on your gpu, so choose your settings accordingly in the config file (lower the FPS if it gets overloaded).
By default, put any shaders into the "Shaders" folder, they will be cycled through every minute by default.

To convert a shader from shader toy to this:
    Change void mainImage(out vec4 fragColor, in vec2 fragCoord) to just void main()
    Replace fragColor with gl_FragColor and fragCoord with gl_FragCoord
    Add the following to the beginning of the file:
        uniform vec2 iResolution;
        uniform float iTime;
        uniform vec2 iMouse;
Shader toy shaders that you convert cannot have anything in the iChannels.
