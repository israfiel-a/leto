/**
 * @file Renderer.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides Leto's rendering interface, which handles the creation
 * of secondary rendering threads, context copying, most OpenGL things, and
 * more.
 * @date 2024-08-25
 *
 * @copyright (c) 2024 - the Leto Team
 */

#ifndef __LETO__RENDERER__
#define __LETO__RENDERER__

#include <stdbool.h>

void CreateRenderer(void);
void DestroyRenderer(void);

//! temp
void render(void);

#endif // __LETO__RENDERER__
