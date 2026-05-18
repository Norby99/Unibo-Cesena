# Introduction

This project aims to implement an autonomous control algorithm that enables a mobile robot to navigate toward a light source while safely detecting and avoiding obstacles within its environment.

# Implementation

## Conceptual Design

Initially, the architecture was conceived to utilize two decentralized, self-managing behaviors:

- **Follow:** Focused on identifying and tracking the light source.
- **Fear:** Focused on detecting and avoiding environmental obstacles.

In the early stages, the goal was to make these behaviors fully autonomous, allowing them to activate or deactivate dynamically based on sensor thresholds (e.g., proximity readings or light intensity). However, this approach introduced significant coordination challenges, particularly when the robot found itself simultaneously close to both an obstacle and the light source. Managing behavior conflicts under this decentralized paradigm proved highly complex and lacked scalability for adding future behaviors.

To address these limitations, a modular architecture was adopted. Each behavior was isolated and developed independently: the *Follow* behavior was tested in an obstacle-free arena, while the *Fear* behavior was tuned by ignoring the light source entirely. Behavior arbitration was then delegated to a high-level supervisor. By decoupling the execution logic, individual behaviors remain entirely independent and unaware of one another.

## Behaviors

### Follow

The robot reads its ambient light sensors and calculates the average light intensity perceived on its left and right sides. The difference between these two averages determines the relative direction of the light source. This difference is utilized by a proportional control mechanism to adjust the trajectory. Specifically, the robot slows down the wheel on the side perceiving less light, smoothly steering its chassis toward the light source.

### Fear

The *Fear* behavior operates on a similar proportional logic but utilizes proximity sensors instead of light sensors, completely ignoring the rear sensors. Rather than navigating toward the maximum stimulus, it actively steers away from it. The algorithm computes the average proximity values for both the left and right sensor arrays. The difference indicates the relative position of the obstacle. To steer away, the robot slows down the wheel on the side where the obstacle is more prominent. If the difference between the left and right sensors is negligible (meaning the path ahead is clear or symmetric), the robot moves forward in a straight line at maximum velocity.

## Controller

The controller is implemented as a Finite State Machine (FSM) that manages the robot's operational states using three distinct modes:

- `STATE_SEARCH_LIGHT`: The robot executes the *Follow* algorithm to locate the light source.
- `STATE_AVOID_OBSTACLE`: The robot prioritizes the *Fear* algorithm to maneuver away from nearby obstacles.
- `STATE_AT_LIGHT`: The robot has successfully reached its destination and stops.

The robot initializes in the `STATE_SEARCH_LIGHT` state. At each control step, the supervisor checks for nearby obstacles. If an obstacle is detected, the FSM transitions to `STATE_AVOID_OBSTACLE`. Once the sensors clear, it reverts to `STATE_SEARCH_LIGHT`. Concurrently, the controller monitors the total perceived light intensity; if it exceeds a predefined threshold (`AT_LIGHT_THRESHOLD`), the robot transitions to `STATE_AT_LIGHT` and halts all motor activity.

```lua
function step()
    n_steps = n_steps + 1
    log("[controller] State: " .. current_state .. " | Step: " .. n_steps)
    update_state()

    if current_state == STATE_SEARCH_LIGHT then
        fsm:follow(robot.light, FOLLOW_ROTATION_FACTOR)
    end

    if current_state == STATE_AVOID_OBSTACLE then
        fsm:fear(robot.proximity, FEAR_ROTATION_FACTOR)
    end
end

function update_state()
    if current_state == STATE_SEARCH_LIGHT and sense_obstacles() then
        log("[controller] Obstacle detected! Switching to AVOID_OBSTACLE state.")
        current_state = STATE_AVOID_OBSTACLE
    end

    if current_state == STATE_AVOID_OBSTACLE and not sense_obstacles() then
        log("[controller] No more obstacles! Switching back to SEARCH_LIGHT state.")
        current_state = STATE_SEARCH_LIGHT
    end

    if sense_light() > AT_LIGHT_THRESHOLD then
        log("[controller] At the light source! Stopping.")
        robot.wheels.set_velocity(0, 0)
        current_state = STATE_AT_LIGHT
    end
end

```

# Observations

## Environmental Variations

To evaluate the robustness of the system, experiments were conducted by altering the vertical position (height) of the light source:

- **Increasing the height:** This modification yielded no significant changes in performance, as the sensors maintained a clear line of sight to the source.
- **Decreasing the height:** Lowering the source drastically increased the time required to complete the task. In rare instances, the robot failed to locate the light source entirely.

This edge-case failure occurs because a lower light position heavily reduces the sensors' perception. When the robot cannot detect a clear light gradient, it defaults to moving forward in a straight line while strictly relying on the *Fear* algorithm to handle environmental boundaries.

## The Role of Sensor Noise

To mitigate the deadlock issues observed when the light source was low, introducing controlled sensor noise proved highly effective.

Initially, the development focus was directed toward filtering out environmental noise. This was attempted by calculating moving averages of the sensor data and comparing them with readings from previous steps to smooth out the robot's trajectory.

However, experimental observations revealed a counter-intuitive behavior: sensor noise acts as a beneficial stochastic mechanism rather than a detriment.

By injecting random perturbations into the sensor readings, the robot avoids getting stuck in local minima or deadlocks when wedged against walls. This noise introduces minor trajectory variations, effectively allowing the robot to "shake free" from tight spaces and resume its search for the light source.

## Methodology Reflection

As previously highlighted, the decoupled development paradigm was fundamental to the success of this architecture. Developing the *Follow* behavior first in a completely wall-free environment allowed for the precise tuning of the light-tracking proportional controller.

Following this, the *Fear* behavior was engineered independently in an environment without light targets, focusing solely on obstacle avoidance and random exploration. This modular workflow ensured that both behaviors were highly optimized and reliable before being integrated under the state machine supervisor.

While the initial instinct was to implement them both together, designing a composite behavior from scratch forces the system to simultaneously manage conflicting sensory inputs. This often leads to unpredictable emergent behaviors that are exceptionally difficult to debug.

## The Role of Memory in the Controller

The controller implemented in this project utilizes a Finite State Machine, meaning it does require memory in the form of an internal state variable (`current_state`). This minimal memory is crucial because it gives the robot context. Without it, the robot could suffer from rapid, high-frequency oscillations when positioned exactly where the attraction of the light balances the repulsion of an obstacle.

While the current state-based memory is sufficient to complete the task, introducing a historical memory (such as a buffer storing the robot's coordinates or sensor readings over the last *N* steps) would significantly improve the system. Historical memory would allow the robot to actively detect if it is stuck in a local minimum and dynamically adjust its strategy to escape environmental deadlocks.

## Performance and Robustness Analysis

The final implementation demonstrates high efficiency, with the robot consistently reaching the light source within a range of 600 to 850 steps across multiple test runs (approximately 30 trials). This narrow range strongly suggests a low standard deviation, indicating high reliability under nominal environmental conditions.

To evaluate the robustness and scalability of the proposed solution, several stress tests were conducted. These included:

- Increasing the sensor noise parameters.
- Introducing additional walls to complicate the arena layout.
- Deploying multiple robots simultaneously within the same environment.

As expected, these complex scenarios increased the average number of steps required for the robot to navigate and locate the light source, primarily due to the increased frequency of obstacle-avoidance maneuvers and potential crowding. However, despite the heightened environmental complexity, the controller successfully completed the task in every trial, proving its resilience and effectiveness.