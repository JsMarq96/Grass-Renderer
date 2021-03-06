//
// Created by jsmar on 21/05/2021.
//

#include "mesh.h"

// NOTE: this only works for TRIANGLE ONLY meshes
void sMesh::load_mesh(const char* mesh_dir) {
    FILE *mesh_file;
    int mesh_file_size;
    char *raw_mesh_file;

    mesh_file = fopen(mesh_dir, "r");

    assert(mesh_file != NULL && "Failed to open the mesh");

    // First gen the vertex/ faces count
    int v_count = 0;
    int f_count = 0;

    int readed_chars;
    char *line_buffer = NULL;
    size_t len = 0;
    while((readed_chars = getline(&line_buffer, &len, mesh_file)) != -1) {
        if (line_buffer[0] == 'v' && line_buffer[1] == ' ') {
            v_count++;

        } else if (line_buffer[0] == 'f') {
            f_count++;
        }
    }

    //info("v count: %i", v_count);
    // Allocate the memmory
    vertex_list = (sGlVertex*) malloc(sizeof(sGlVertex) * v_count);
    faces_index = (unsigned int*) malloc(sizeof(float) * f_count * 3);
    indices_cout = f_count * 3;

    //info("!!indices count %i vertex count : %i", result->indices_cout, v_count);

    // Rewind and then load the data
    rewind(mesh_file);

    struct sUV_Wrapper {
        float u;
        float v;
    };

    int vertex_index = 0;
    int uv_count = 0;
    int faces_index_count = 0;
    sUV_Wrapper *tmp_uvs = NULL;

    // Since the OBJs are stored in a sequential fashin, we can just get the
    // number of indexes, and allocate stuff for the UVs, temporally store the UVs
    // and merge everything while processing the faces
    while((readed_chars = getline(&line_buffer, &len, mesh_file)) != -1) {
        if (line_buffer[0] == 'v' && line_buffer[1] == ' ') {
            float x, y, z;
            sscanf(line_buffer,"v %f %f %f\n",&x, &y, &z);
            vertex_list[vertex_index].x = x;
            vertex_list[vertex_index].y = y;
            vertex_list[vertex_index].z = z;
            vertex_index++;
        } else if (line_buffer[0] == 'v' && line_buffer[1] == 't') {
            if (uv_count == 0) {
                tmp_uvs = (sUV_Wrapper*) malloc(sizeof(sUV_Wrapper) * (v_count));
            }
            float u,v;
            sscanf(line_buffer, "vt %f %f\n", &u, &v);

            // Store the UVs by tuples
            tmp_uvs[uv_count].u = u;
            tmp_uvs[uv_count].v =  1.f - v;
            uv_count++;
        } else if (line_buffer[0] == 'f') {
            unsigned int index1, index2, index3, normal1, normal2, normal3, uv1, uv2, uv3;
            sscanf(line_buffer,
                   "f %i/%i/%i %i/%i/%i %i/%i/%i\n",
                   &index1,
                   &uv1,
                   &normal1,
                   &index2,
                   &uv2,
                   &normal2,
                   &index3,
                   &uv3,
                   &normal3);

            index1 -= 1;
            index2 -= 1;
            index3 -= 1;
            uv1    -= 1;
            uv2    -= 1;
            uv3    -= 1;

            if (index2 == 0) {
                int p = 0;
            }

            faces_index[faces_index_count++] = index1;
            faces_index[faces_index_count++] = index2;
            faces_index[faces_index_count++] = index3;

            vertex_list[index1].u = tmp_uvs[uv1].u;
            vertex_list[index1].v = tmp_uvs[uv1].v;

            vertex_list[index2].u = tmp_uvs[uv2].u;
            vertex_list[index2].v = tmp_uvs[uv2].v;

            vertex_list[index3].u = tmp_uvs[uv3].u;
            vertex_list[index3].v = tmp_uvs[uv3].v;

            if (index2 == 0) {
                int p = 0;
            }
        }
    }

    free(tmp_uvs);

    fclose(mesh_file);
    vertex_count = vertex_index;
    raw_vertex_size = vertex_index * 5;
    uv_count = uv_count;
};

void sMesh::load_cube() {
    float tmp_vertex[36 * 3] = {-1.0f,  1.0f, -1.0f,
                           -1.0f, -1.0f, -1.0f,
                           1.0f, -1.0f, -1.0f,
                           1.0f, -1.0f, -1.0f,
                           1.0f,  1.0f, -1.0f,
                           -1.0f,  1.0f, -1.0f,

                           -1.0f, -1.0f,  1.0f,
                           -1.0f, -1.0f, -1.0f,
                           -1.0f,  1.0f, -1.0f,
                           -1.0f,  1.0f, -1.0f,
                           -1.0f,  1.0f,  1.0f,
                           -1.0f, -1.0f,  1.0f,

                           1.0f, -1.0f, -1.0f,
                           1.0f, -1.0f,  1.0f,
                           1.0f,  1.0f,  1.0f,
                           1.0f,  1.0f,  1.0f,
                           1.0f,  1.0f, -1.0f,
                           1.0f, -1.0f, -1.0f,

                           -1.0f, -1.0f,  1.0f,
                           -1.0f,  1.0f,  1.0f,
                           1.0f,  1.0f,  1.0f,
                           1.0f,  1.0f,  1.0f,
                           1.0f, -1.0f,  1.0f,
                           -1.0f, -1.0f,  1.0f,

                           -1.0f,  1.0f, -1.0f,
                           1.0f,  1.0f, -1.0f,
                           1.0f,  1.0f,  1.0f,
                           1.0f,  1.0f,  1.0f,
                           -1.0f,  1.0f,  1.0f,
                           -1.0f,  1.0f, -1.0f,

                           -1.0f, -1.0f, -1.0f,
                           -1.0f, -1.0f,  1.0f,
                           1.0f, -1.0f, -1.0f,
                           1.0f, -1.0f, -1.0f,
                           -1.0f, -1.0f,  1.0f,
                           1.0f, -1.0f,  1.0f
    };
    raw_vertex_list = (float *) malloc(sizeof(tmp_vertex));
    memcpy(raw_vertex_list, tmp_vertex, sizeof(tmp_vertex));
    raw_vertex_size = sizeof(tmp_vertex);
    vertex_count = 36;
}


void sMesh::destroy() {
    free(raw_vertex_list);
    free(faces_index);
};

sVector3
get_bounding_box_dimensions(const sMesh *mesh,
                            const sVector3 scale) {
    sVector3 max_shape{};

    for(int i = 0; i < mesh->vertex_count; i++) {
        max_shape.x = MAX(mesh->vertex_list[i].x, max_shape.x);
        max_shape.y = MAX(mesh->vertex_list[i].y, max_shape.y);
        max_shape.z = MAX(mesh->vertex_list[i].z, max_shape.z);
    }

    max_shape.x *= scale.x;
    max_shape.y *= scale.y;
    max_shape.z *= scale.z;

    return max_shape;
}
