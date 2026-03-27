-- Put your global variables here
MAX_VELOCITY = 15	-- The maximum velocity of the robot, in pixels per second

--[[ The maximum values perceived by the robot's sensors, used for normalizing the
sensor values. These values can be adjusted based on the specific characteristics of
the robot's sensors and the environment it operates in.
]]
MAX_LIGHT_PERCIVED = 0.3	
MAX_PROXIMITY_PERCIVED = 1.0
MIN_MOTOR_GROUND_PERCIVED = 0.01

-- Imports
local MotorSchemaController = require("libraries.motor_schema_controller")
local Follow = require("libraries.behaviors.follow")
local Fear = require("libraries.behaviors.fear")
local StopAtDarkSpot = require("libraries.behaviors.stop_at_dark_spot")

local db = require("libraries.roboto_debug")

n_steps = 0

function init()
	robot.wheels.set_velocity(0, 0)
	n_steps = 0
	robot.leds.set_all_colors("black")

	controller = MotorSchemaController.new(MAX_VELOCITY)
    --controller:add(StopAtDarkSpot.new(MAX_VELOCITY, robot.motor_ground, MIN_MOTOR_GROUND_PERCIVED))
	controller:add(Fear.new(MAX_VELOCITY, robot.proximity, MAX_PROXIMITY_PERCIVED))
	--controller:add(Follow.new(MAX_VELOCITY, robot.light, MAX_LIGHT_PERCIVED))
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
