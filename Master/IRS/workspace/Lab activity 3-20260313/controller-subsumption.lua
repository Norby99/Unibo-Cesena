-- Put your global variables here
MAX_VELOCITY = 15
MAX_LIGHT_PERCIVED = 0.3
MAX_PROXIMITY_PERCIVED = 1.0
MIN_MOTOR_GROUND_PERCIVED = 0.01

--[[
TODO:
- IMPORTANT: here i use the fusion, but the professors want to see the subsumption,
	So when i higher priority behavior is sure, it has the full controll
- clear up the code
- set some variables to global constants
]]

-- Imports
local Behavior = require("libraries.behaviors.behavior")
local Follow = require("libraries.behaviors.follow")
local Fear = require("libraries.behaviors.fear")
local StopAtDarkSpot = require("libraries.behaviors.stop_at_dark_spot")

local db = require("libraries.roboto_debug")

n_steps = 0

function init()
	robot.wheels.set_velocity(0, 0)
	n_steps = 0
	robot.leds.set_all_colors("black")

	controller = Behavior.new(MAX_VELOCITY)
	controller:add(StopAtDarkSpot.new(MAX_VELOCITY, robot.motor_ground, MIN_MOTOR_GROUND_PERCIVED))
	controller:add(Fear.new(MAX_VELOCITY, robot.proximity, MAX_PROXIMITY_PERCIVED))
	controller:add(Follow.new(MAX_VELOCITY, robot.light, MAX_LIGHT_PERCIVED))
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
