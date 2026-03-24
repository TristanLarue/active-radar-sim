# Missile–Jet Engagement Simulation

This repository contains a **3D physics simulation of aerial combat** between a fighter jet and guided missiles. The current implementation (1.0) is a Python-based prototype with six-degree-of-freedom rigid-body dynamics, aerodynamic forces, control surfaces, and reinforcement learning for the jet pilot. Visualization runs at 60 Hz with optional real-time display or headless training over many epochs.

---

## What the project is about

- **Physics**: Aerodynamic model (lift, drag, control surfaces), barometric air density, angle-of-attack/sideslip, numerical integration with Rodrigues rotation and SVD re-orthonormalization.
- **Entities**: Jet (throttle + pitch/roll/yaw, driven by a neural network) and missiles (guidance strategies, proximity fusing).
- **Simulation**: 60 Hz tick loop, configurable epoch length (e.g. 40 s), phase-based difficulty (more missiles over time).
- **AI**: Jet pilot trained with reinforcement learning (e.g. Soft Actor–Critic) to evade missiles.
- **Viz**: 3D view of trajectories and an optional force/orientation diagram (currently VPython).

So in short: **real-time-capable jet vs missile engagement sim with a learned jet pilot**, aimed at believable dynamics and scalable training.

---

## Next version (2.0) — vision and new aspects

The following describes the **general idea of the next version** of the project, as planned in the radar/2.0 roadmap. Version 2.0 is conceived as a **full C++ rewrite** with richer physics, radar/sensors, and stricter “no cheating” entity logic.

### Performance and scope

- **Full C++ implementation** for much faster epochs. Target: **1 epoch per ~10 ms** at 24 Hz, with **40 seconds of “realtime” simulation** per epoch, enabling heavy parallel learning.
- **Parallel learning** with **multiple jets and missiles** in the same or parallel environments.

### Radar, waves, and signatures

- **Radar waves and signatures**: Radar is no longer abstract; the sim will model radar waves and target signatures.
- **Full SAM-site behaviour**: Ground-based SAM sites with **ground radar wave decluttering** and **missile communication** (e.g. mid-course updates, handover).
- **Mesh-aware signatures**: Radar signature will depend on **mesh geometry and orientation** (e.g. precomputed or LUT-based), so aspect angle and shape matter.
- **Entity perspective / no cheating**: **Full entity-perspective system**: no direct use of `target_entity.position`. Each aircraft, missile, or drone must use **its own radar (and possibly other sensors)** to detect and track targets. This makes the sim suitable for realistic guidance and countermeasures.

### Environment

- **General wind**: A **global wind field** applied to all entities (affecting trajectory and possibly sensors).

### Physics and aerodynamics (more general and mesh-based)

- **Full mesh interactivity**:
  - **Pre-rendered CFD with LUTs** for aerodynamic forces (no real-time CFD).
  - **Dynamic radar signature** from the same (or related) mesh representation; radar model will be **hand-built**.
- **More general, modular entity dynamics** so that **every aircraft, missile, and drone** can be implemented consistently (no special-case hacks).
- **Removed from 1.0** in 2.0:
  - **AoA-based lift/drag coefficient curves** (replaced by mesh/LUT-based forces).
  - **Flat ground**.
  - **Non-mesh simplifications** for physics (forces come from mesh/LUT).
  - **Current missile simplifications** (e.g. direct orientation toward target); missiles will use radar and proper dynamics.

### Allowed libraries (C++ stack)

- **Magnum Engine**: rendering and visualization.
- **MLPack**: jet AI pilot reinforcement learning (e.g. Soft Actor–Critic).
- **Standard / basic**: e.g. iostream, vector, dynamic containers (e.g. list/vector for entities), cmath.

---

## Open design questions (from the roadmap)

- **Control surface torques**: With mesh/LUT-based forces, torques must come from the mesh. Options: multiple meshes per control deflection (e.g. discrete -1/0/1), same mesh + empirical torques, or dynamic mesh deformation (risky for pre-rendered CFD). A **dynamic system with many meshes per elevator/rudder/aileron** and a tolerance, each with its own LUT, is one candidate.
- **Angular acceleration**: Keep or drop? Full angular dynamics would need more CFD or damping modelling; **instant velocity-based** control can retain most realism except in stall. Damping could be baked into mesh interactions.
- **Missile “pitbull”**: Should missiles have autonomous terminal guidance (e.g. onboard seeker)? Requires a **modular radar wave system** and would support diverse missile and drone types.
- **Radar wave implementation**:
  - Raycasting vs other methods?
  - How realistic (e.g. shape and orientation dependent)?
  - Precomputed radar cross-section / signature vs runtime model?
  - Deeper **research** on how radar waves and detection are to be modelled.

---

## Summary

- **Now (1.0)**: Python jet–missile engagement sim with 6-DOF physics, learned pilot, and simple guidance.
- **Next (2.0)**: C++ rewrite with fast epochs, **radar waves and signatures**, **entity-only perception (no cheating)**, **wind**, **mesh-based CFD LUTs**, and **modular dynamics** for jets, missiles, and drones, with SAM sites and missile communication in the loop.

README2.md ties the **current project** to this **next-version vision** so the radar/wind and “no cheating” aspects are clear as the general direction for v2.
