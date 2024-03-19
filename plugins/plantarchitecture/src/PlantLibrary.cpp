/** \file "PlantLibrary.cpp" Contains routines for loading and building plant models from a library of predefined plant types.

    Copyright (C) 2016-2024 Brian Bailey

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, version 2.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

*/

#include "PlantArchitecture.h"

using namespace helios;

void PlantArchitecture::loadPlantModelFromLibrary( const std::string &plant_label ){

    current_plant_model = plant_label;
    initializeDefaultShoots(plant_label);

}

uint PlantArchitecture::buildPlantInstanceFromLibrary( const helios::vec3 &base_position, float age ){

    if( current_plant_model.empty() ){
        helios_runtime_error("ERROR (PlantArchitecture::buildPlantInstanceFromLibrary): current plant model has not been initialized from library. You must call loadPlantModelFromLibrary() first.");
    }

    uint plantID = 0;
    if( current_plant_model == "almond" ) {
        plantID = buildAlmondTree(base_position, age);
    }else if( current_plant_model == "bean" ) {
        plantID = buildBeanPlant(base_position, age);
    }else if( current_plant_model == "cowpea" ) {
        plantID = buildCowpeaPlant(base_position, age);
    }else{
        assert(true); //shouldn't be here
    }

    return plantID;

}

ShootParameters PlantArchitecture::getCurrentShootParameters( const std::string &shoot_type_label ){

    if( shoot_types.find(shoot_type_label) == shoot_types.end() ){
        helios_runtime_error("ERROR (PlantArchitecture::getCurrentShootParameters): shoot type label of " + shoot_type_label + " does not exist in the current shoot parameters.");
    }

    return shoot_types.at(shoot_type_label);
}

std::map<std::string, ShootParameters> PlantArchitecture::getCurrentShootParameters(){
    return shoot_types;
}

void PlantArchitecture::updateCurrentShootParameters( const std::string &shoot_type_label, const ShootParameters &params ){
    shoot_types[shoot_type_label] = params;
}

void PlantArchitecture::updateCurrentShootParameters( const std::map<std::string, ShootParameters> &params ){
    shoot_types = params;
}

void PlantArchitecture::initializeDefaultShoots( const std::string &plant_label ){

    if( plant_label == "almond" ) {
        initializeAlmondTreeShoots();
    }else if( plant_label == "bean" ) {
        initializeBeanShoots();
    }else if( plant_label == "cowpea" ) {
        initializeCowpeaShoots();
    }else{
        helios_runtime_error("ERROR (PlantArchitecture::loadPlantModelFromLibrary): plant label of " + plant_label + " does not exist in the library.");
    }

}

void PlantArchitecture::initializeAlmondTreeShoots(){

    // ---- Phytomer Parameters ---- //

    PhytomerParameters phytomer_parameters_almond(context_ptr->getRandomGenerator());

    phytomer_parameters_almond.internode.pitch = 0;
    phytomer_parameters_almond.internode.length_segments = 1;
    phytomer_parameters_almond.internode.color = make_RGBcolor(0.6,0.45,0.15);

    phytomer_parameters_almond.petiole.petioles_per_internode = 1;
    phytomer_parameters_almond.petiole.pitch = -50;
    phytomer_parameters_almond.petiole.taper = 0.1;
    phytomer_parameters_almond.petiole.curvature.uniformDistribution(-3000,3000);
    phytomer_parameters_almond.petiole.length = 0.001;
    phytomer_parameters_almond.petiole.radius = 0.00025;
    phytomer_parameters_almond.petiole.length_segments = 7;

    phytomer_parameters_almond.leaf.leaves_per_petiole = 1;
    phytomer_parameters_almond.leaf.leaflet_offset = 0.2;
    phytomer_parameters_almond.leaf.leaflet_scale = 0.7;
    phytomer_parameters_almond.leaf.prototype_function = AlmondLeafPrototype;
    phytomer_parameters_almond.leaf.prototype_scale = 0.015;

    phytomer_parameters_almond.inflorescence.length_segments = 10;
    phytomer_parameters_almond.inflorescence.flower_prototype_function = AlmondFlowerPrototype;
    phytomer_parameters_almond.inflorescence.flower_prototype_scale = 0.01;
    phytomer_parameters_almond.inflorescence.fruit_prototype_function = AlmondFruitPrototype;
    phytomer_parameters_almond.inflorescence.fruit_pitch.uniformDistribution(-35,0);
    phytomer_parameters_almond.inflorescence.fruit_prototype_scale = 0.008;
    phytomer_parameters_almond.inflorescence.fruit_roll = 90;
    phytomer_parameters_almond.inflorescence.flower_arrangement_pattern = "alternate";
    phytomer_parameters_almond.inflorescence.flowers_per_rachis.uniformDistribution(1, 3);
    phytomer_parameters_almond.inflorescence.flower_offset = 0.14;
    phytomer_parameters_almond.inflorescence.curvature = -300;
    phytomer_parameters_almond.inflorescence.length = 0.0;
    phytomer_parameters_almond.inflorescence.rachis_radius = 0.0005;

    // ---- Shoot Parameters ---- //

    // Trunk
    ShootParameters shoot_parameters_trunk(context_ptr->getRandomGenerator());
    shoot_parameters_trunk.phytomer_parameters = phytomer_parameters_almond;
    shoot_parameters_trunk.max_nodes = 20;
    shoot_parameters_trunk.girth_growth_rate = 1.02;
    shoot_parameters_trunk.internode_radius_initial = 0.005;
    shoot_parameters_trunk.bud_break_probability = 1;
    shoot_parameters_trunk.bud_time = 0;
    shoot_parameters_trunk.tortuosity = 1000;
    shoot_parameters_trunk.defineChildShootTypes({"scaffold"},{1});
    shoot_parameters_trunk.phyllochron = 100;

    // Proleptic shoots
    ShootParameters shoot_parameters_proleptic(context_ptr->getRandomGenerator());
    shoot_parameters_proleptic.phytomer_parameters = phytomer_parameters_almond;
    shoot_parameters_proleptic.max_nodes = 36;
    shoot_parameters_proleptic.phyllochron.uniformDistribution(1,1.1);
    shoot_parameters_proleptic.phyllotactic_angle.uniformDistribution( 130, 145 );
    shoot_parameters_proleptic.elongation_rate = 0.04;
    shoot_parameters_proleptic.girth_growth_rate = 1.02;
    shoot_parameters_proleptic.bud_break_probability = 0.75;
    shoot_parameters_proleptic.bud_time = 0;
    shoot_parameters_proleptic.gravitropic_curvature.uniformDistribution(180,210);
    shoot_parameters_proleptic.tortuosity = 60;
    shoot_parameters_proleptic.internode_radius_initial = 0.00075;
    shoot_parameters_proleptic.child_insertion_angle_tip.uniformDistribution( 35, 45);
    shoot_parameters_proleptic.child_internode_length_max = 0.005;
    shoot_parameters_proleptic.child_internode_length_min = 0.0005;
    shoot_parameters_proleptic.child_internode_length_decay_rate = 0.0025;
    shoot_parameters_proleptic.fruit_set_probability = 0.5;
    shoot_parameters_proleptic.flower_probability = 0.75;
    shoot_parameters_proleptic.flowers_require_dormancy = true;
    shoot_parameters_proleptic.growth_requires_dormancy = true;
    shoot_parameters_proleptic.defineChildShootTypes({"sylleptic","proleptic"},{0.,1.});

    // Sylleptic shoots
    ShootParameters shoot_parameters_sylleptic = shoot_parameters_proleptic;
    shoot_parameters_sylleptic.phytomer_parameters.leaf.prototype_scale = 0.025;
    shoot_parameters_sylleptic.bud_break_probability = 1;
    shoot_parameters_sylleptic.gravitropic_curvature.uniformDistribution(250,300);
    shoot_parameters_sylleptic.child_internode_length_max = 0.01;
    shoot_parameters_sylleptic.flowers_require_dormancy = true;
    shoot_parameters_sylleptic.growth_requires_dormancy = true; //seems to not be working when false
    shoot_parameters_proleptic.defineChildShootTypes({"sylleptic"},{1.0});

    // Main scaffolds
    ShootParameters shoot_parameters_scaffold = shoot_parameters_proleptic;
    shoot_parameters_scaffold.gravitropic_curvature.uniformDistribution(50,70);
    shoot_parameters_scaffold.phyllochron = 0.9;
    shoot_parameters_scaffold.child_internode_length_max = 0.015;
    shoot_parameters_scaffold.tortuosity = 20;

    defineShootType("trunk", shoot_parameters_trunk);
    defineShootType("scaffold", shoot_parameters_scaffold);
    defineShootType("proleptic", shoot_parameters_proleptic);
    defineShootType("sylleptic", shoot_parameters_sylleptic);

}

uint PlantArchitecture::buildAlmondTree(const helios::vec3 &base_position, float age) {

    if( shoot_types.empty() ){
        //automatically initialize almond tree shoots
        initializeAlmondTreeShoots();
    }

    uint plantID = addPlantInstance(base_position, age);

    uint uID_trunk = addBaseStemShoot(plantID, 3, 0.1, make_AxisRotation(context_ptr->randu(0.f, 0.05f * M_PI), context_ptr->randu(0.f, 2.f * M_PI), 0.f * M_PI), "trunk");

    plant_instances.at(plantID).shoot_tree.at(uID_trunk)->meristem_is_alive = false;

    auto phytomers = plant_instances.at(plantID).shoot_tree.at(uID_trunk)->phytomers;
    for( const auto & phytomer : phytomers ){
        phytomer->removeLeaf();
        phytomer->flower_bud_state = BUD_DEAD;
        phytomer->vegetative_bud_state = BUD_DEAD;
    }

    uint Nscaffolds = context_ptr->randu(4,5);

    for( int i=0; i<Nscaffolds; i++ ) {
        uint uID_shoot = appendShoot(plantID, uID_trunk, context_ptr->randu(6, 8), 0.02, make_AxisRotation(context_ptr->randu(deg2rad(35), deg2rad(45)), (float(i) + context_ptr->randu(-0.1f, 0.1f)) / float(Nscaffolds) * 2 * M_PI, 0), "scaffold");

        plant_instances.at(plantID).shoot_tree.at(uID_shoot)->breakDormancy();

        uint blind_nodes = context_ptr->randu(2,3);
        for( int b=0; b<blind_nodes; b++){
            if( b<plant_instances.at(plantID).shoot_tree.at(uID_shoot)->phytomers.size() ) {
                plant_instances.at(plantID).shoot_tree.at(uID_shoot)->phytomers.at(b)->removeLeaf();
                plant_instances.at(plantID).shoot_tree.at(uID_shoot)->phytomers.at(b)->flower_bud_state = BUD_DEAD;
                plant_instances.at(plantID).shoot_tree.at(uID_shoot)->phytomers.at(b)->vegetative_bud_state = BUD_DEAD;
            }
        }

    }

    setPlantPhenologicalThresholds(plantID, 1, 1, 3, 7, 12);

    return plantID;

}

void PlantArchitecture::initializeCowpeaShoots() {

    // ---- Phytomer Parameters ---- //

    PhytomerParameters phytomer_parameters_cowpea(context_ptr->getRandomGenerator());

    phytomer_parameters_cowpea.internode.pitch = 20;
    phytomer_parameters_cowpea.internode.color = make_RGBcolor(0.38, 0.48, 0.1);
    phytomer_parameters_cowpea.internode.length_segments = 5;

    phytomer_parameters_cowpea.petiole.petioles_per_internode = 1;
    phytomer_parameters_cowpea.petiole.pitch = 45;
    phytomer_parameters_cowpea.petiole.radius = 0.001;
    phytomer_parameters_cowpea.petiole.length = 0.02;
    phytomer_parameters_cowpea.petiole.taper = 0.15;
    phytomer_parameters_cowpea.petiole.curvature = -100;
    phytomer_parameters_cowpea.petiole.length_segments = 5;

    phytomer_parameters_cowpea.leaf.leaves_per_petiole = 3;
    phytomer_parameters_cowpea.leaf.pitch.normalDistribution(0, 20);
    phytomer_parameters_cowpea.leaf.yaw = 0;
    phytomer_parameters_cowpea.leaf.roll.normalDistribution( 0, 10);
    phytomer_parameters_cowpea.leaf.leaflet_offset = 0.3;
    phytomer_parameters_cowpea.leaf.leaflet_scale = 0.9;
    phytomer_parameters_cowpea.leaf.prototype_function = CowpeaLeafPrototype;
    phytomer_parameters_cowpea.leaf.prototype_scale = 0.04;

    phytomer_parameters_cowpea.inflorescence.length_segments = 10;
    phytomer_parameters_cowpea.inflorescence.fruit_prototype_function = CowpeaFruitPrototype;
    phytomer_parameters_cowpea.inflorescence.fruit_prototype_scale = 0.05;
    phytomer_parameters_cowpea.inflorescence.flower_prototype_scale = 0.05;
    phytomer_parameters_cowpea.inflorescence.flower_arrangement_pattern = "opposite";
    phytomer_parameters_cowpea.inflorescence.flowers_per_rachis = 4;
    phytomer_parameters_cowpea.inflorescence.flower_offset = 0.2;
    phytomer_parameters_cowpea.inflorescence.curvature = -200;
    phytomer_parameters_cowpea.inflorescence.length = 0.025;
    phytomer_parameters_cowpea.inflorescence.rachis_radius = 0.001;

    // ---- Shoot Parameters ---- //

    ShootParameters shoot_parameters_trifoliate(context_ptr->getRandomGenerator());
    shoot_parameters_trifoliate.phytomer_parameters = phytomer_parameters_cowpea;
    shoot_parameters_trifoliate.bud_break_probability = 0.25;
    shoot_parameters_trifoliate.internode_radius_initial = 0.001;
    shoot_parameters_trifoliate.phyllochron = 1;
    shoot_parameters_trifoliate.elongation_rate = 0.01;
    shoot_parameters_trifoliate.gravitropic_curvature = 200;
    shoot_parameters_trifoliate.bud_time = 3;
    shoot_parameters_trifoliate.phyllotactic_angle.uniformDistribution(145, 215);
    shoot_parameters_trifoliate.child_internode_length_max = 0.01;
    shoot_parameters_trifoliate.child_internode_length_decay_rate = 0;
    shoot_parameters_trifoliate.child_insertion_angle_tip = 0.4*M_PI;
    shoot_parameters_trifoliate.flowers_require_dormancy = false;
    shoot_parameters_trifoliate.growth_requires_dormancy = false;
    shoot_parameters_trifoliate.flower_probability = 0.6;
    shoot_parameters_trifoliate.defineChildShootTypes({"trifoliate"},{1.0});
    shoot_parameters_trifoliate.max_nodes = 20;

//    defineShootType("unifoliate",shoot_parameters_unifoliate);
    defineShootType("trifoliate",shoot_parameters_trifoliate);

}

uint PlantArchitecture::buildCowpeaPlant(const helios::vec3 &base_position, float age) {

    if (shoot_types.empty()) {
        //automatically initialize cowpea plant shoots
        initializeCowpeaShoots();
    }

    uint plantID = addPlantInstance(base_position, age);

    uint uID_stem = addBaseStemShoot(plantID, 1, 0.01, make_AxisRotation(context_ptr->randu(0.f, 0.01f * M_PI), context_ptr->randu(0.f, 2.f * M_PI), 0.f * M_PI), "trifoliate");

    breakPlantDormancy(plantID);

    setPlantPhenologicalThresholds(plantID, 0, 20, 60, 5, 100);

    return plantID;

}

void PlantArchitecture::initializeBeanShoots() {

    // ---- Phytomer Parameters ---- //

    PhytomerParameters phytomer_parameters_bean(context_ptr->getRandomGenerator());

    phytomer_parameters_bean.internode.pitch = 20;
    phytomer_parameters_bean.internode.color = make_RGBcolor(0.38, 0.48, 0.1);
    phytomer_parameters_bean.internode.length_segments = 5;

    phytomer_parameters_bean.petiole.petioles_per_internode = 1;
    phytomer_parameters_bean.petiole.pitch = 45;
    phytomer_parameters_bean.petiole.radius = 0.001;
    phytomer_parameters_bean.petiole.length = 0.02;
    phytomer_parameters_bean.petiole.taper = 0.15;
    phytomer_parameters_bean.petiole.curvature = -100;
    phytomer_parameters_bean.petiole.length_segments = 5;

    phytomer_parameters_bean.leaf.leaves_per_petiole = 3;
    phytomer_parameters_bean.leaf.pitch.normalDistribution(0, 20);
    phytomer_parameters_bean.leaf.yaw = 0;
    phytomer_parameters_bean.leaf.roll.normalDistribution( 0, 10);
    phytomer_parameters_bean.leaf.leaflet_offset = 0.3;
    phytomer_parameters_bean.leaf.leaflet_scale = 0.9;
    phytomer_parameters_bean.leaf.prototype_function = BeanLeafPrototype;
    phytomer_parameters_bean.leaf.prototype_scale = 0.04;

    phytomer_parameters_bean.inflorescence.length_segments = 10;
    phytomer_parameters_bean.inflorescence.fruit_prototype_function = BeanFruitPrototype;
    phytomer_parameters_bean.inflorescence.fruit_prototype_scale = 0.05;
    phytomer_parameters_bean.inflorescence.flower_prototype_scale = 0.05;
    phytomer_parameters_bean.inflorescence.flower_arrangement_pattern = "opposite";
    phytomer_parameters_bean.inflorescence.flowers_per_rachis = 4;
    phytomer_parameters_bean.inflorescence.flower_offset = 0.2;
    phytomer_parameters_bean.inflorescence.curvature = -200;
    phytomer_parameters_bean.inflorescence.length = 0.025;
    phytomer_parameters_bean.inflorescence.rachis_radius = 0.001;

    // ---- Shoot Parameters ---- //

    ShootParameters shoot_parameters_trifoliate(context_ptr->getRandomGenerator());
    shoot_parameters_trifoliate.phytomer_parameters = phytomer_parameters_bean;
    shoot_parameters_trifoliate.bud_break_probability = 0.25;
    shoot_parameters_trifoliate.internode_radius_initial = 0.001;
    shoot_parameters_trifoliate.phyllochron = 1;
    shoot_parameters_trifoliate.elongation_rate = 0.01;
    shoot_parameters_trifoliate.gravitropic_curvature = 200;
    shoot_parameters_trifoliate.bud_time = 3;
    shoot_parameters_trifoliate.phyllotactic_angle.uniformDistribution(145, 215);
    shoot_parameters_trifoliate.child_internode_length_max = 0.01;
    shoot_parameters_trifoliate.child_internode_length_decay_rate = 0;
    shoot_parameters_trifoliate.child_insertion_angle_tip = 0.4*M_PI;
    shoot_parameters_trifoliate.flowers_require_dormancy = false;
    shoot_parameters_trifoliate.growth_requires_dormancy = false;
    shoot_parameters_trifoliate.flower_probability = 0.6;
    shoot_parameters_trifoliate.defineChildShootTypes({"trifoliate"},{1.0});
    shoot_parameters_trifoliate.max_nodes = 20;

//    defineShootType("unifoliate",shoot_parameters_unifoliate);
    defineShootType("trifoliate",shoot_parameters_trifoliate);


}

uint PlantArchitecture::buildBeanPlant(const helios::vec3 &base_position, float age) {

    if (shoot_types.empty()) {
        //automatically initialize bean plant shoots
        initializeBeanShoots();
    }

    uint plantID = addPlantInstance(base_position, age);

    uint uID_stem = addBaseStemShoot(plantID, 1, 0.01, make_AxisRotation(context_ptr->randu(0.f, 0.01f * M_PI), context_ptr->randu(0.f, 2.f * M_PI), 0.f * M_PI), "trifoliate");

    breakPlantDormancy(plantID);

    setPlantPhenologicalThresholds(plantID, 0, 20, 60, 5, 100);

    return plantID;

}