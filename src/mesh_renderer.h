//
// Created by jsmar on 21/05/2021.
//

#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H
#include <GL/gl3w.h>

#include "camera.h"
#include "shader.h"
#include "mesh.h"
#include "math.h"
#include "material.h"
#include "raw_shaders.h"

struct sMeshRenderer {
    unsigned int    VAO;
    unsigned int    VBO;
    unsigned int    EBO;

    const sMesh     *origin_mesh;
    int             indices_count = 0;

    sShader         shader;

    void render_init(const sMesh    *raw_mesh,
                     const bool      is_static);

    void render_init_cube(const bool is_static);

    void render_mesh(const sMat44        &model_mat,
                     const sMat44        &viewproj_mat,
                     const sCamera       &camera,
                     const bool          show_wireframe) const;

    void render_destroy();
};


struct sInstancedMeshRenderer {
    unsigned int    VAO             = -1;
    unsigned int    mesh_VBO        = -1;
    unsigned int    mesh_EBO        = -1;
    unsigned int    instanced_VBO   = -1;

    sMat44  *instanced_models = NULL;
    unsigned int instance_size = 0;

    const sMesh     *origin_mesh;
    int             indices_count = 0;

    void init(const sMesh &raw_mesh,
              const bool is_static);

    void set_instances(sMat44 *models_list, const unsigned int count);

    void destroy();

    void render_meshes(const sMaterial &material,
                       const sCamera   &camera,
                       const bool       show_wireframe) const;
};

#endif //MESH_RENDERER_H
