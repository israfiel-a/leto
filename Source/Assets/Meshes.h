/**
 * @file Meshes.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides a public interface to create, load, and manage textured
 * meshes. Rendering, however, is not for this interface, instead take a
 * look at @file Renderer.h.
 * @date 2024-08-30
 *
 * @copyright (c) 2024 - the Leto Team
 * This document is under the GNU Affero General Public License v3.0. It
 * can be modified and distributed (commercially or otherwise) freely, and
 * can be used privately and within patents. No liability or warranty is
 * guaranteed. However, on use, the user must state license and copyright,
 * any changes made, and disclose the source of the document. For more
 * information see the @file LICENSE.md file included with this
 * distribution of the Leto source code.
 */

#ifndef __LETO__MESHES__
#define __LETO__MESHES__

#include <vec3.h>

typedef enum
{
    color_no_ambient,
    color_and_ambient,
    highlight,
    reflection_and_raytrace,
    glass_and_raytrace,
    fresnel_and_raytrace,
    refraction_and_fresnel_and_raytrace,
    refraction_and_raytrace,
    reflection_no_raytrace,
    glass_no_raytrace,
    shadows_on_invisible
} illumination_t;

typedef struct
{
    vec4 specular; // first three are spec, last is spec exponent
    vec3 diffuse;
    vec3 ambient;
    float transparency;
    vec3 transmission_filter; // only on transparent obj
    float refraction;
    illumination_t illumination;
    const char* name;
} material_t;

typedef struct
{
    vec3* vertices;
    vec3* normals;
    vec3* texture;
    vec3* faces;
    material_t* materials;
    const char* name;
} mesh_t;

mesh_t* LoadMesh(const char* name);
void UnloadMesh(mesh_t* mesh);

#endif // __LETO__MESHES__
