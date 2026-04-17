local nn = require "nn"

MAX_VELOCITY = 15

function init()
	genome_param = os.getenv("GENOME")
	genome = {}
	for num in genome_param:gmatch("[^,]+") do
			table.insert(genome,tonumber(num))
	end
	net = nn.createNetwork(24,2,genome)
end


function step()
	local input = {}
	for i=1,#robot.light do
		input[i] = robot.light[i].value
	end
	output = nn.forward(net,input)
	left_v = output[1] * MAX_VELOCITY
	right_v = output[2] * MAX_VELOCITY
	robot.wheels.set_velocity(left_v,right_v)
end



function destroy()
  -- Output the Euclidean distance from the light
  -- DEPENDENCE: argos file
  x = robot.positioning.position.x
  y = robot.positioning.position.y
  d = math.sqrt((x-2)^2 + y^2)
  dmax = 6 -- sqrt(3^2+5^2)
  fitness = (dmax-d)/dmax
  print(string.format("FITNESS:%.6f", fitness))
end
