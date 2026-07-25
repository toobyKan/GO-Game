# Release: 07:25:2026
1. Rendering & Graphics Decoupling

    Removed: SDLRenderer direct dependency on Board and GameLogic.

    Added: IGraphicsRenderer interface to define primitive drawing commands (drawLine, drawFilledCircle, setColor).

    Added: Drawable_Entity interface representing objects that can draw themselves using an IGraphicsRenderer.

    Added: BoardView class responsible for translating logical board state into pixel coordinates and issuing draw commands.

    Changed: SDLRenderer now solely manages the SDL window, context, and executes raw drawing primitives for a list of Drawable_Entity objects.

2. Core Game Logic & Event System

    Replaced: Subject and Observer base classes with Modern C++ callback system using std::function. GameLogic now exposes an onTurnCompleted vector of callbacks.

    Removed: Multiple inheritance from SDLRenderer.

    Changed: The rendering update is now triggered via a lambda function in main.cpp bound to the game logic callback, preserving strict domain isolation.

3. Data Structure Optimization

    Changed: Board internal representation migrated from a 2D std::vector<std::vector<Stone>> to a contiguous 1D std::vector<Stone>.

    Added: Row-major index mapping (y * width + x) for coordinate translation.

    Changed: Updated GameLogic Ko rule history tracking (previousState_) to match the new 1D contiguous layout.

4. Input Handling

    Added: InputController class to handle raw OS events.

    Changed: Moved the responsibility of translating screen pixel coordinates into logical grid coordinates out of the main.cpp event loop and into InputController.