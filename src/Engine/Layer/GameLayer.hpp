// | -------------------------------
#pragma once
// | -------------------------------

namespace ENG
{
  /**
   * @brief Base class that connects a game with the engine.
   *
   * GameLayer defines the hooks the engine calls during its lifetime:
   * initialization, input, update, fixed update, render and shutdown. A game
   * derives from GameLayer and overrides the hooks it needs; every hook has
   * an empty default implementation, so none of them is mandatory.
   *
   * The game layer is given to the Engine at construction and is owned by it.
   *
   * Usage:
   * @code
   * class MyGame : public ENG::GameLayer
   * {
   *   public:
   *     void OnInit(void) override { }
   *     void OnUpdate(float dt) override { }
   *     void OnRender(float dt) override { }
   * };
   * @endcode
   */
  class GameLayer
  {
    public:
      /// @brief Virtual destructor, so the game layer is destroyed correctly.
      virtual ~GameLayer() = default;

      /**
       * @brief Called once when the game starts.
       *
       * Override it to load resources and create the initial scene.
       */
      virtual void OnInit(void){};

      /**
       * @brief Called once when the game is closing.
       *
       * Override it to release the resources of the game.
       */
      virtual void OnDestroy(void){};

      /**
       * @brief Tells whether the game is still running.
       *
       * The engine keeps running while this returns true. To quit the
       * game, set the `gl_Done` member to true.
       *
       * @return true while the game is running, false once it is done.
       */
      virtual bool IsRunning(void){return !gl_Done;};

      /**
       * @brief Called to process the inputs of the game.
       *
       * @param[in] dt Time elapsed since the previous frame, in seconds.
       */
      virtual void OnInputs(float dt){(void)dt;};

      /**
       * @brief Called once per frame to update the game logic.
       *
       * @param[in] dt Time elapsed since the previous frame, in seconds.
       */
      virtual void OnUpdate(float dt){(void)dt;};

      /**
       * @brief Called at a fixed time step to update the game logic.
       *
       * Meant for logic that needs a constant time step, such as physics.
       *
       * @param[in] dt Duration of the fixed time step, in seconds.
       */
      virtual void OnUpdateFixed(float dt){(void)dt;};

      /**
       * @brief Called once per frame to draw the game.
       *
       * @param[in] dt Time elapsed since the previous frame, in seconds.
       */
      virtual void OnRender(float dt){(void)dt;};

    protected:
      /// @brief Set to true to tell the engine that the game is done and
      ///        must close. IsRunning() returns its opposite.
      bool gl_Done;
  };
}
