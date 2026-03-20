# Subsumption Architecture Controller Report

## Introduction
This project implements a robot controller based on the **Subsumption Architecture**. The goal of the controller is to manage different primitive behaviors and coordinate them to achieve complex navigation tasks. Instead of delving into the inner workings of each specific behavior, this report focuses on the overarching architecture, the design choices made for the controller, and the limitations observed during the project.

## System Architecture: The Top-Down Approach
Traditional Subsumption Architecture often relies on a bottom-up flow where lower-level behaviors are computed continuously, and higher-level behaviors can inhibit or "subsume" their outputs. 

In this project, we adopted a slightly different, more efficient approach. While the core logic of prioritizing behaviors remains the same, the controller evaluates the layers from **top to bottom** (from highest priority to lowest). Instead of computing all levels and letting the higher ones override the lower ones—which becomes computationally inefficient as the number of layers grows—our controller cascaded down the hierarchy and stops at the very first layer that requires action. Once a high-priority behavior claims control, lower-priority behaviors are not evaluated for that execution step.

The following snippet from the controller's main loop demonstrates this early-exit logic:

```lua
function SubsumptionController:run()
    local left_speed = self.max_velocity
    local right_speed = self.max_velocity

    -- Iterate sequentially over behaviors (from highest to lowest priority)
    for _, b in ipairs(self.behaviors) do
        local action_left, action_right = b:action()

        -- As soon as a behavior returns valid actions, it claims control
        -- and the loop returns abruptly, ignoring lower-priority behaviors.
        if action_left ~= nil and action_right ~= nil then
            left_speed, right_speed = action_left, action_right
            return left_speed, right_speed
        end
    end

    return left_speed, right_speed
end
```

### Behavior Layers
The controller is composed of the following layers, ordered from highest to lowest priority:

1. **Stop at Dark Spot (Highest Priority):** Ensures the robot stops immediately when a dark spot/hole is detected to prevent falling or failing the primary safety constraint.
2. **Fear (Obstacle Avoidance):** Reacts to proximity sensors to steer the robot away from walls and obstacles.
3. **Follow (Light Attraction, Lowest Priority):** Drives the robot towards light sources when no imminent danger is detected.

**Default Fallback:** If none of the behaviors reach their activation threshold, the controller defaults to simply driving the robot straight forward. This naive forward-motion strategy works effectively in this scenario because the robot is operating in a closed rectangular arena—meaning it is guaranteed to eventually interact with an obstacle or detect a light source. However, if the robot were deployed in an open-world environment, it would be much more appropriate to implement a smarter fallback behavior, such as a "random walk," to prevent the robot from traveling endlessly in a single direction while improving area exploration.

## Implementation and Extensibility
The architecture's implementation is handled within the main Lua script. One of the primary advantages of this design is how intuitively simple it is to add a new layer. Because the controller abstracts the execution loop entirely, extending the robot's capabilities merely requires instantiating a new behavior and passing it to the central `controller` object via the `add()` method. As long as behaviors are appended in decreasing order of priority, no structural modifications to the core controller logic are needed.

```lua
local SubsumptionController = require("libraries.subsumption_controller")
local Follow = require("libraries.behaviors.follow")
local Fear = require("libraries.behaviors.fear")
local StopAtDarkSpot = require("libraries.behaviors.stop_at_dark_spot")

function init()
-- Initialize the controller and define the hierarchy bounds
controller = SubsumptionController.new(MAX_VELOCITY)

-- Level 1: Highest priority - Safety stop
controller:add(StopAtDarkSpot.new(MAX_VELOCITY, robot.motor_ground, MIN_MOTOR_GROUND_PERCIVED))

-- Level 2: Medium priority - Obstacle avoidance
controller:add(Fear.new(MAX_VELOCITY, robot.proximity, MAX_PROXIMITY_PERCIVED, FEAR_ACTIVATION_THRESHOLD))

-- Level 3: Lowest priority - Goal seeking (Light)
controller:add(Follow.new(MAX_VELOCITY, robot.light, MAX_LIGHT_PERCIVED, FOLLOW_ACTIVATION_THRESHOLD))
end

function step()
-- The controller:run() method handles the top-down subsumption logic.
-- It checks behaviors in the order they were added, returning the 
-- velocities of the first active behavior.
local left_vel, right_vel = controller:run()

robot.wheels.set_velocity(left_vel, right_vel)
end
```

## Limitations of the Architecture
Through the development and testing of this project, clear limitations of the Subsumption Architecture emerged, primarily due to its **exclusive** nature.

Because the system strictly follows the logic of a single behavior at any given time, it is highly susceptible to behavior monopolization. If a scenario arises where a high-priority behavior is continuously triggered, the robot will entirely focus on that single task, ignoring everything else. 

For example, if the robot finds itself trapped in a corridor or between two walls, the **Fear** (obstacle avoidance) behavior will activate continuously. Consequently, the robot will permanently dedicate its actions to avoiding the walls and will never execute the **Follow** behavior. Even if there is a light source explicitly guiding it out, the strict exclusive prioritization prevents the goal-seeking logic from contributing to the final motor output, effectively "getting the robot stuck" in its own safety mechanism.
