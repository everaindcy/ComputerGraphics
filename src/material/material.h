#ifndef MATERIAL_H
#define MATERIAL_H

#include "../util/utils.h"
#include "../object/hittable.h"
#include "../texture/textures.h"

struct hit_record;

class material {
public:
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const = 0;
};

#endif