-- Put your global variables here
MAX_VELOCITY = 15
MAX_LIGHT_PERCIVED = 0.3
MAX_PROXIMITY_PERCIVED = 1.0

--[[
TODO:
- clear up the code
- set some variables to global constants
]]

-- Imports
local Behavior = require("libraries.behaviors.behavior")
local Follow = require("libraries.behaviors.follow")
local Fear = require("libraries.behaviors.fear")

local db = require("libraries.roboto_debug")

n_steps = 0

--[[ This function is executed every time you press the 'execute'
     button ]]
function init()
	robot.wheels.set_velocity(0, 0)
	n_steps = 0
	robot.leds.set_all_colors("black")

	controller = Behavior.new()
	controller:add(Follow.new(10, 1, robot.light, MAX_LIGHT_PERCIVED))
	controller:add(Fear.new(10, 1, robot.proximity, MAX_PROXIMITY_PERCIVED))
end

function step()
	n_steps = n_steps + 1

	local left_vel, right_vel = controller:run(robot.sensors)

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
