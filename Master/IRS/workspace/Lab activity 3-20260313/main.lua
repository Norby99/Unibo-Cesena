-- Put your global variables here
MAX_VELOCITY = 15
MAX_LIGHT_PERCIVED = 0.3
MAX_PROXIMITY_PERCIVED = 1.0
MIN_MOTOR_GROUND_PERCIVED = 0.01

FEAR_ACTIVATION_THRESHOLD = 0.03
FOLLOW_ACTIVATION_THRESHOLD = 0.03

--[[
TODO:
- set some variables to global constants
]]

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
	controller:add(Fear.new(MAX_VELOCITY, robot.proximity, MAX_PROXIMITY_PERCIVED, FEAR_ACTIVATION_THRESHOLD))
	controller:add(Follow.new(MAX_VELOCITY, robot.light, MAX_LIGHT_PERCIVED, FOLLOW_ACTIVATION_THRESHOLD))
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
