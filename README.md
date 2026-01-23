# Physics Engine 

---

## Demo


https://github.com/user-attachments/assets/ca7d8441-d8c4-4e95-8b00-31d073ee4762


---

## Overview

This is a 2D physics engine built around a scene–object–component architecture (inspiration is a mix of both unity and unreal engine workflow)



- A Scene owns and updates all physics objects
- Each Physics Object has a transform and a list of components (colliders only as of now)
- A Spatial Hash Grid is used to speed up collision checks





---

## Core Concepts

### 1. Scene (`GScene`)

The scene contains the world.

- Stores all physics objects
- Updates physics
- Interfaces with the spatial hash for collision detection

All physics objects must be created and registered through the scene.

---

### 2. Transform (`GTransform`)

Each physics object has a `GTransform`, which defines its transform

It has

- Position (x, y)
- Rotation
- Scale (not implimented yet)

The transform is shared by all components (colliders as of now) attached to the object.

The `GTransform` is used for collision detection

---

### 3. GObjects

A physics object represents an object in the scene.

- Has a `GTransform`
- Contains a vector of components
- Components are  colliders as of now


```
Object
 |-- GTransform
 |-- Physics related values (velocity, friction etc)
 |-- Components[] 
```

This allows:

- Multiple colliders per object
- Flexible shapes for a single entity



---

### 4. Components (Colliders only as of now)

- Stored in a `std::vector` inside the physics object

Types of colliders:

- Polygon collider
- Circle collider

---

### 5. Spatial Hashing (`GSpatialHash`)

To avoid O(n²) collision checks, the engine uses spatial hashing.

- Divides the world into a fixed grid
- Each collider is inserted into one or more grid cells
- Collision checks are only performed within nearby cells

#### Configuration

The spatial hash system uses **global constants** defined in its header, such as:

- Maximum number of colliders
- Cell size
- Expected average number cells occupied by a collider
- Max buckets (max number of cells occupied at a time)

I was initially going for a linked list approach which has no limit to number of objects but i wanted to run this on a microcontroller and linked lists are not good for small memory devices.

---

## &#x20;Physics Update Loop

A typical physics update cycle looks like this:

1. Scene update begins
2. All colliders are inserted into the spatial hash grid
3. Collision pairs in the same grid cell are checked 
4. Narrow-phase collision checks are performed using SAT (Separated Axis Theorem)
5. Collision responses are applied

This design keeps collision detection scalable as object count increases.

---

# How To Use

### Step 1: Create a Scene

```cpp
GScene *myScene;
myScene = new GScene();
```



### Shape Templates (ColliderShapeTemplate)

Before creating colliders, you define **shape templates**. A shape template stores raw geometry data and is shared across multiple colliders to avoid repeated memory allocations when the same shape is reused.

Square shape example

```cpp
ColliderShapeTemplate shapeTemplate1;
shapeTemplate1.numPoints = 4;
shapeTemplate1.points = (Vec2D*)malloc(4 * sizeof(Vec2D));

shapeTemplate1.points[0] = Vec2D(-10, -10);
shapeTemplate1.points[1] = Vec2D(-10,  10);
shapeTemplate1.points[2] = Vec2D( 10,  10);
shapeTemplate1.points[3] = Vec2D( 10, -10);
```
The template should be a convex shape. If convex shape is needed, separate the shape into convex shapes and add them as two colliders.

This template is **not owned by the collider**. It acts as a reference that multiple colliders can point to.

---

### Step 2: Creating Objects and Colliders

Objects are created through the scene and components are added to them.

Reference to the shape template is given.

```cpp
auto* newObj = myScene->AddNewObject<GObject>();

auto* coll = newObj->CreateComponent<GColliderComp>(
    GColliderComp::ColliderType::Polygon,
    25, //radius
    Vec2D(2,3) //center of circle in local coordinates of the object
);

//or for circle
auto* coll = newObj->CreateComponent<GColliderComp>(
    GColliderComp::ColliderType::Circle,
    &shapeTemplate1
);
```

- Each object can have multiple collider components
- The collider stores a reference to the shape template

---

### Step 3: Enabling Physics (RigidBody vs Static)

Physics behavior is controlled per object.

```cpp
newObj->setPhysicsEnabled(true);
```

- **Physics disabled + collider present** → Object is treated as **static**
- **Physics enabled** → Object behaves as a **rigid body**

Static objects participate in collision detection but do not move or respond to forces.

---

### Step 4: Setting Physical Properties

When physics is enabled, the object acts as a rigid body and responds to collisions:

```cpp
newObj->transform.position = Vec2D(-2500, 350);
newObj->transform.rotation = 0.5f; //in radians
newObj->velocity           = Vec2D(550, 0);
newObj->friction           = 0.9f;
newObj->mass               = 70.0f;
newObj->momentOfInertia    = 250000.0f;
```


The update loop will handle the rest

---

## Rendering

- SFML is used only for visualization
- Physics objects expose transform data
- Rendering systems simply read position and rotation

The physics engine itself has **no dependency on rendering logic** and can be reused independently.





## TODO

- Create a collision notification system so that objects can get to know when they encounter a collision.
  



