// Like 1.0, except adding:
// - FULL C++ IMPLEMENTATION FOR FASTER EPOCHS: Final goal is 1 epoch per 10ms at 24hz with 40 seconds of "realtime" simulation.
// - Radar waves & signature including full SAM sites behavior with ground radar wave decluttering and missile communication
// - Full mesh interactivity (pre-rendered CFD with LUT, dynamic radar waves signature). CFD will be library based, radar waves will be fully hand built
// - PERLIN NOISE TERRAIN: will be calculated in collision detection and radar waves obstruction. Will be fully hand built for fun
// - General wind, applied to all entities
// - More general & modular entity dynamics in order for every aircraft/missiles/drones to work properly
// - FULL ENTITY PERSPECTIVE SYSTEM: No more cheating with target_entity.position. Each entity must use their radar to properly detect & track a target
// - Parallel learning with multiple jets & missiles

// Will remove from 1.0:
// - aoa based lift & drag coefficient calculation
// - Flat ground
// - Non-mesh reliant physics calculation/simplifications
// - Missile simplifications

// Allowed libraries:
// - Magnum Engine: Rendering Vizualisation (Since I dont care about the actual vizualisation)
// - MLPack: For Jet AI pilot reinforcement learning using Soft Actor Critic
// Necessary Basic Libraries
// - iostream: Input Output interactions
// - vector: For every 3D vectors like position, orientation, velocity, acceleration, torque (altough I could code my own, mainly to save time)
// - ArrayList: Mainly for dynamic entity storing
// - cmath: For all basic math functions

// Remaining questions:
// HOW TO APPLY CONTROL SURFACE TORQUES!!!
// - Do we have multiple meshes for each control surface possibilities? This would lock inputs to -1/0/1 instead of float
// - Do we keep the same mesh but add extra forces for control surfaces as simplifications?
// - Do we Dynamically Change the meshes shape? (Would break the CFD pre-render)
// - (Since all forces will now be mesh/triangle based, we can't simply *add* a force onto the jet)
// - I think we can use a dynamic system that has plenty of meshes for each possible elevator/rudder/aileron possibilities with a tolerance, each with their own pre-rendered LUT
// Do I keep angular acceleration?
// - Would require dynamic CFD calculations in order to have proper damping
// - Instant velocity based system doesnt really lose that much realism fidelity unless entities are in stall
// - Could simplify damping and keep angular acceleration to allow stall, damping could be introduced into the mesh interactions
// Do we add missile pitbull capabilities?
// - Would require a more modular radar wave system
// - Would allow for all variety of missiles & drone to be implemented
// How to implement the radar wave system *!*
// - Raycasting?
// - How realistic do we really want the waves to be?
// - Realistic enough so the shape of the jet and its orientation matters
// - Do we precalculate radar signature and use that model for radar calculation?
// - How do radars waves actually work...
// - *Research must be done here =====
