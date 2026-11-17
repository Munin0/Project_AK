// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Object/ObjectPool.hpp"
#include "Engine/Render/Batching/Batch.hpp"
// | -------------------------------
#include <cstdint>
// | -------------------------------

/**
 * @file PoolSystems.hpp
 * @brief Systems that run every object of an ObjectPool: update, pointer, render.
 *
 * Free functions, not ObjectPool methods: the pool is only a container and
 * does not need to know the Batcher, the camera or the input. A scene calls
 * them on its m_pool:
 * @code
 * void MyScene::Inputs(float dt) { ENG::DispatchPointer(m_pool); ... }
 * void MyScene::Update(float dt) { ENG::UpdateObjects(m_pool, dt); }
 * void MyScene::Render(ENG::Batcher& b) { ENG::RenderObjects(m_pool, b); }
 * @endcode
 *
 * @warning While any of them runs, objects must not be added to or removed
 *          from the pool (from Update(), OnPointer() or Draw()).
 */
namespace ENG
{
  /**
   * @brief Calls Object::Update() on every object of the pool.
   *
   * Hidden objects are updated too (SetVisible() only affects drawing).
   *
   * @param[in,out] pool Objects to update.
   * @param[in]     dt   Time elapsed since the previous frame, in seconds.
   */
  [[maybe_unused]] void UpdateObjects(ObjectPool& pool, float dt);

  /**
   * @brief Gives the mouse to the objects of the pool, from the top down.
   *
   * Calls Object::OnPointer() on every visible object, in the reverse of the
   * draw order (top layer first), so the UI gets the mouse before the world.
   * Each object gets the mouse in its own RenderSpace: screen pixels for
   * Screen objects, converted with the batcher's active camera for World
   * objects (as they are drawn).
   *
   * It does not consume anything itself: an object takes a click with
   * PollEvent::ConsumeMouseButtonPress(), and from then on the objects below
   * and the world no longer see it. Call it at the start of the scene's
   * Inputs(), before the scene reads the mouse:
   * @code
   * void MyScene::Inputs(float dt)
   * {
   *   ENG::DispatchPointer(m_pool);                          // UI first
   *   if (ENG::PollEvent::Get().IsMouseButtonPress(SDL_BUTTON_LEFT))
   *     MovePlayerTo(...);                                   // only if nobody took it
   * }
   * @endcode
   * Not calling it disables the pointer for the objects (cutscenes...).
   *
   * @param[in,out] pool Objects that receive the pointer.
   */
  [[maybe_unused]] void DispatchPointer(ObjectPool& pool);

  /**
   * @brief Draws every object of the pool, ordered by layer, space and shader.
   *
   * Uses ObjectPool::Sort(); the queue is cached and only rebuilt after
   * objects are added or removed. Hidden objects are skipped. World objects
   * are drawn with the batcher's active camera, Screen objects without a
   * camera; the active camera is restored at the end.
   *
   * @param[in,out] pool Objects to draw.
   * @param[in,out] b    Batcher that collects the draw data of this frame.
   */
  [[maybe_unused]] void RenderObjects(ObjectPool& pool, Batcher& b);

  /**
   * @brief Draws the objects of the pool whose layer is in [minLayer, maxLayer).
   *
   * Same range rule as SceneMap::Render(), so a scene can interleave both,
   * e.g. world objects, then foreground tiles, then the UI on top:
   * @code
   * m_map.Render(b, LAYER_GROUND, LAYER_PLAYER);
   * ENG::RenderObjects(m_pool, b, LAYER_GROUND, LAYER_UI_BG);   // world
   * m_map.Render(b, LAYER_PLAYER, LAYER_MAX);
   * ENG::RenderObjects(m_pool, b, LAYER_UI_BG, LAYER_MAX);      // UI
   * @endcode
   * Otherwise the same as RenderObjects(pool, b).
   *
   * @param[in,out] pool     Objects to draw.
   * @param[in,out] b        Batcher that collects the draw data of this frame.
   * @param[in]     minLayer First layer to draw (included).
   * @param[in]     maxLayer Layer where the range ends (excluded).
   */
  [[maybe_unused]] void RenderObjects(ObjectPool& pool, Batcher& b, uint8_t minLayer, uint8_t maxLayer);
}
