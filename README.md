# IRC client
 A bare-bones IRC client - My first "real" GUI application, using SDL 2.0 with a custom framework.

I was curious if I could make a simple UI framework from scratch, so I went ahead and tried to do it without reading any guides on how. The result is very basic and no use for anything practical, but was fun to make. Learned a bit, tried out network sockets.

All Components of the UI framework are contained within the UI folder, and can be separated from the rest of the repository. 

#### Things to maybe try in the future

- Redo text rendering. Right now the framework uses SDL_TTF to do text rendering, but this seems to be a bit off. Might also try working with the underlying rendering library directly instead of through the SDL wrapper. 
- A large number of IRC codes just dump the results to the "SERVER INFO" channel. Maybe find something better to do with them?
- Hover/Click effects?

#### Building


Just build the "IRC Client" VS project. I haven't used a build system, so right now anything in the res folder will be copied to the output directory. 

### UI Overview

The simplest application using the framework defines an extension of the Application class, defines it's startup and shutdown behavior, creates an instance of it, and then runs it. Windows can be added by the Window class and AddWindow function. Windows have a tree of elements to draw and (maybe) interact with. Current defined elements include:

- Rectangle. Draws a rectangle
- Text. Draws text. Optionally editable
- Scroller. Allows for vertical scrolling of contained elements.
- Button. Runs defined callback on click. 
- Image. Draws an image from a file

Do define custom Elements, simply extend the Element class, defining how it should draw, and optionally how it should handle clicks and other events. Elements can set themselves as the focus in the window, which will send further events not handled by the window to the element. 

