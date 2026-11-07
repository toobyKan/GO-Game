# Go Game Project Release Changelog

## Release: 11.07.2026

1. State Management

    Added: AppState interface for defining common state lifecycle, event, update, and rendering behavior.

    Added: StateManager with stack-based state transitions.

    Added: Support for replacing, resetting, pushing, and popping states.

    Added: Pause and resume lifecycle handling for stacked states.

    Added: Main menu, local game, and pause states.

2. Menu and Button UI

    Added: Button drawable entity with bounds, hover detection, text labels, and callback actions.

    Added: Main menu buttons for starting a local game and exiting the application.

    Added: Pause menu buttons for resuming, returning to the main menu, and exiting.

    Added: Responsive button layout updates when the window is resized.

3. Text and Renderer Improvements

    Added: SDL_ttf integration for rendering menu and button text.

    Added: drawFilledRect and drawText primitives to the graphics renderer interface.

    Added: Font loading support in SDLRenderer.

    Changed: UI elements now render through the graphics renderer abstraction.

4. Game Loop Timing

    Added: ``std::chrono::steady_clock`` timing for calculating elapsed frame time.

    Changed: State updates now receive measured delta time instead of a fixed update value.

    Added: Delta-time clamping to prevent unusually large updates after application stalls.

5. Project Organization and Build System

    Changed: State headers were moved into the headers/states directory.

    Changed: State implementations were moved into the src/states directory.

    Changed: Include paths now use the headers directory consistently.

    Changed: The Makefile now discovers source files in nested directories.

    Changed: The application executable and test binaries are generated inside the build directory.

6. Testing

    Added: State-manager tests covering state replacement, reset, push, and pop behavior.

    Added: Additional board-geometry edge-case tests.

    Changed: Tests are now organized inside the tests directory.

## Release: 10.31.2026

1. Responsive Board Geometry

    Added: BoardGeometry class for calculating the board resize and centering logic.

    Changed: Board rendering now uses intersection spacing instead of fixed tile sizing.

    Added: Automatic board centering and scaling for different window dimensions.

    Changed: Mouse coordinates are converted using the same geometry as rendering to keep input and visuals synchronized.

2. Window Resizing

    Changed: SDL window is now resizable.

    Added: Handling for SDL_WINDOWEVENT_SIZE_CHANGED events.

    Changed: BoardView and InputController update their geometry when the window is resized.

    Added: Support for both wide and tall window layouts while keeping the board square.

3. Board Validation and Game State Management

    Added: Validation for positive board sizes.

    Added: Safe restoration of complete board state after invalid suicide or Ko moves.

    Changed: Capture processing now occurs before suicide validation.

    Changed: Ko state is updated after every successful move.

4. Input and Move Validation

    Added: Rejection of negative and out-of-range board coordinates.

    Added: Rejection of Stone::None placements.

    Added: Rejection of moves made by the wrong player.

    Changed: Invalid moves no longer modify the board or advance the turn.

5. Testing and Build System

    Added: Game-logic tests covering placement, turn order, suicide, captures, invalid moves, and Ko.

    Added: Makefile test target.

    Changed: Test executable is generated inside the build directory.

    Changed: Makefile now uses the headers directory and includes test cleanup support.

## Release: 07.25.2026

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

    Changed: Board internal representation migrated from a 2D ``std::vector<std::vector<Stone>>`` to a contiguous 1D ``std::vector<Stone>``.

    Added: Row-major index mapping (y * width + x) for coordinate translation.

    Changed: Updated GameLogic Ko rule history tracking (previousState_) to match the new 1D contiguous layout.

4. Input Handling

    Added: InputController class to handle raw OS events.

    Changed: Moved the responsibility of translating screen pixel coordinates into logical grid coordinates out of the main.cpp event loop and into InputController.
