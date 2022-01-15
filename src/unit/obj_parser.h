#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include "../util/utils.h"
#include "../object/objects.h"
#include "../texture/textures.h"
#include "../material/materials.h"

#include <iostream>

hittable_list parse_obj(const string file_obj, const string mtl_path,
                        bool use_vn = true, bool use_vt = true,
                        shared_ptr<material> default_mat = make_shared<lambertian>(color(0,0,0))
                        ) {    
    tinyobj::ObjReaderConfig config;
    config.mtl_search_path = mtl_path;

    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(file_obj, config)) {
        if (!reader.Error().empty()) {
            std::cerr << "Error: " << reader.Error();
        }
        exit(1);
    }
    if (!reader.Warning().empty()) {
        std::cerr << "Warning: " << reader.Warning();
    }

    auto &attrib = reader.GetAttrib();
    auto &shapes = reader.GetShapes();
    auto &materials = reader.GetMaterials();

    std::vector<shared_ptr<material> > mats;

    for (const auto &mat : materials) {
        if(mat.diffuse_texname.empty()) {
            mats.push_back(make_shared<lambertian>(color(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2])));
        } else {
            mats.push_back(make_shared<lambertian>(make_shared<image_texture>(mat.diffuse_texname.c_str())));
        }
    }

    hittable_list objects;

    for (const auto &shape : shapes) {
        hittable_list tris;
        size_t indice_idx = 0;

        for (size_t i = 0; i < shape.mesh.num_face_vertices.size(); i++) {
            auto fv = shape.mesh.num_face_vertices[i];
            if (fv != 3) {
                std::cerr << "A triangle has " << fv << " vertices." << std::endl;
                indice_idx += fv;
                continue;
            }
            auto idx_a = shape.mesh.indices[indice_idx+0];
            auto idx_b = shape.mesh.indices[indice_idx+1];
            auto idx_c = shape.mesh.indices[indice_idx+2];
            indice_idx += fv;

            point3 vv[3];
            vec3 vn[3];
            vec3 vt[3];

            vv[0] = {attrib.vertices[3*idx_a.vertex_index+0],
                     attrib.vertices[3*idx_a.vertex_index+1],
                     attrib.vertices[3*idx_a.vertex_index+2]};
            vv[1] = {attrib.vertices[3*idx_b.vertex_index+0],
                     attrib.vertices[3*idx_b.vertex_index+1],
                     attrib.vertices[3*idx_b.vertex_index+2]};
            vv[2] = {attrib.vertices[3*idx_c.vertex_index+0],
                     attrib.vertices[3*idx_c.vertex_index+1],
                     attrib.vertices[3*idx_c.vertex_index+2]};

            if (idx_a.normal_index >= 0) {
                vn[0] = {attrib.vertices[3*idx_a.normal_index+0],
                         attrib.vertices[3*idx_a.normal_index+1],
                         attrib.vertices[3*idx_a.normal_index+2]};
                vn[1] = {attrib.vertices[3*idx_b.normal_index+0],
                         attrib.vertices[3*idx_b.normal_index+1],
                         attrib.vertices[3*idx_b.normal_index+2]};
                vn[2] = {attrib.vertices[3*idx_c.normal_index+0],
                         attrib.vertices[3*idx_c.normal_index+1],
                         attrib.vertices[3*idx_c.normal_index+2]};
            }

            if (idx_a.texcoord_index >= 0) {
                vt[0] = {attrib.vertices[2*idx_a.normal_index+0],
                         attrib.vertices[2*idx_a.normal_index+1],
                         0};
                vt[1] = {attrib.vertices[2*idx_b.normal_index+0],
                         attrib.vertices[2*idx_b.normal_index+1],
                         0};
                vt[2] = {attrib.vertices[2*idx_c.normal_index+0],
                         attrib.vertices[2*idx_c.normal_index+1],
                         0};
            }

            auto mat_idx = shape.mesh.material_ids[i];
            // auto mat_ptr = (mat_idx == -1) ? default_mat : mat_list[mat_id];

            tris.add(make_shared<triangle>(vv[0], vv[1], vv[2],
                                           vn[0], vn[1], vn[2],
                                           vt[0], vt[1], vt[2],
                                           default_mat, use_vn, use_vt));
        }

        auto mh = make_shared<mesh>(tris);
        objects.add(mh);
    }

    return objects;
}

#endif