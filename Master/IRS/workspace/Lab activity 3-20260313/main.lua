-- Put your global variables here
MAX_VELOCITY = 15	-- The maximum velocity of the robot, in pixels per second

--[[ The maximum values perceived by the robot's sensors, used for normalizing the
sensor values. These values can be adjusted based on the specific characteristics of
the robot's sensors and the environment it operates in.
]]
MAX_LIGHT_PERCIVED = 0.3	
MAX_PROXIMITY_PERCIVED = 1.0
MIN_MOTOR_GROUND_PERCIVED = 0.01

--[[ The activation thresholds for the behaviors. These values determine when a
behavior should be activated based on the perceived sensor values. Adjusting these
thresholds can help fine-tune the robot's behavior in different environments.
]]
FEAR_ACTIVATION_THRESHOLD = 0.03
FOLLOW_ACTIVATION_THRESHOLD = 0.02

ROTATION_FACTOR_FEAR = 40.0		-- This factor determines how sharply the robot will turn when the fear behavior is activated.
ROTATION_FACTOR_FOLLOW = 20.0	-- This factor determines how sharply the robot will turn when the follow behavior is activated.

MIN_RELATIVE_DIFF = 0.05	-- The minimum relative difference between the left and right sensor groups for the behaviors to activate.
FEAR_BLIND_SPOT_PERCENTAGE = 0.23	-- The percentage of the front sensors that are considered a blind spot for the fear behavior.

-- Imports
local SubsumptionController = require("libraries.subsumption_controller")
local Follow = require("libraries.behaviors.follow")
local Fear = require("libraries.behaviors.fear")
local StopAtDarkSpot = require("libraries.behaviors.stop_at_dark_spot")

local db = require("libraries.roboto_debug")

n_steps = 0

function init()
	robot.wheels.set_velocity(0, 0)
	n_steps = 0
	robot.leds.set_all_colors("black")

	controller = SubsumptionController.new(MAX_VELOCITY)
	controller:add(StopAtDarkSpot.new(MAX_VELOCITY, robot.motor_ground, MIN_MOTOR_GROUND_PERCIVED))
	controller:add(Fear.new(MAX_VELOCITY, robot.proximity, MAX_PROXIMITY_PERCIVED, FEAR_ACTIVATION_THRESHOLD, ROTATION_FACTOR_FEAR, MIN_RELATIVE_DIFF, FEAR_BLIND_SPOT_PERCENTAGE))
	controller:add(Follow.new(MAX_VELOCITY, robot.light, MAX_LIGHT_PERCIVED, FOLLOW_ACTIVATION_THRESHOLD, ROTATION_FACTOR_FOLLOW, MIN_RELATIVE_DIFF))
end

function step()
	n_steps = n_steps + 1

	local left_vel, right_vel = controller:run()

	robot.wheels.set_velocity(left_vel, right_vel)
end

function reset()
	init()
end

--[[ This function is executed only once, when the robot is removed
     from the simulation ]]
function destroy()
   -- put your code here
end
