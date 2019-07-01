# CoconutCNC
* A G-Code Controller and Visualiser for GRBL Machines.
* Written with C++/ImGui/OpenGL

** CoconutCNC is still in Alpha. Please do not use it in a production environment.**

## Key Functions
* Customisable User Interface. Taylor to a Touch/Mouse Driven Experience.
* Manual Control of Your CNC via MID Commands,
* Jog Buttons.
* Live Monitoring of CNC State.
* Visualising G-code Tool Paths, Tool and Tool Holder in 3D.
* Spindle, Feed Rate and Rapid Rate Override Control.
* Tool Change Alerts

## Planned Features
* Stock & Fixtures in Visualiser
* Tool Path Crash Prediction (Intersecting Remaining Stock/Fixtures)

## System Requirements for Running CoconutCNC
* Windows/Linux/MacOS X
* A Graphics Card with OpenGL 2.0 Support
* Requires GRBL v1.1 firmware. Available here https://github.com/gnea/grbl

## External Build Requirements
* GLFW3
* FreeType
* LibSerialPort https://sigrok.org/wiki/Libserialport

## Depends on (included in src tree, see project for respective licenses)
* ImGui https://github.com/ocornut/imgui
* IconFontCppHelpers https://github.com/juliettef/IconFontCppHeaders
* GLAD https://glad.dav1d.de/
* GLM https://glm.g-truc.net/
* spdlog https://github.com/gabime/spdlog
* nlohmann json https://github.com/nlohmann/json
* NativeFileDialog https://github.com/mlabbe/nativefiledialog

## Extra Credits
* Thanks to the developers & /contributers of Candle, the basis of CoconutCNC. https://github.com/Denvi/Candle.git
* Thanks to https://icons8.com/icon/24979/coconut for the icon.
* Using UI Icons from FontAwesome https://fontawesome.com
