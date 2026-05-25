local nn = require "nn"

GENOME = "0.05695886724842625, 0.7582571712343404, 0.6510833197776225, 0.38635228966353696, -0.14279913999180027, -0.5445510745983546, -0.8931974136639811, 0.5819594803330961, 0.13464142890725284, 0.9382513941860384, -0.34512837760544524, 0.1390837694390452, -0.4432036872544627, 0.9060101410435968, 0.3314902181308981, 0.5279866575610421, -0.655313913593792, 0.5538843419647321, -0.9538592734836917, 0.8759571803968429, 0.28214605442687724, 0.5157638494239691, 0.8695642621825868, -0.7009857839900837, -0.5457050667230057, -0.864406338951492, 0.311153641421402, 0.9810554947094274, -0.9713402266075664, -0.370653813821197, -0.848144856224494, 0.7399078501867402, -0.02335615781659217, -0.7275854169748812, 0.5376871049357352, 0.7678268080389212, 0.08296556156534685, -0.3145858828855428, -0.12118050361042476, 0.8719775050138476, -0.6808182434288592, -0.16752544139224823, 0.6087975731661162, -0.04073086239368351, 0.608050415006752, -0.03789380540976017, 0.8028857295131704, -0.18834768546252878, -0.2358516678403373, -0.4110690365792993"

MAX_VELOCITY = 15
MIN_DISTANCE_TO_LIGHT = 1.2

function init()
	genome = {}
	for num in GENOME:gmatch("[^,]+") do
			table.insert(genome,tonumber(num))
	end
	net = nn.createNetwork(24,2,genome)

	n_step = 0
	is_terminated = false
end

function step()
	n_step = n_step + 1
	x = robot.positioning.position.x
	y = robot.positioning.position.y
	d = math.sqrt((x-2)^2 + y^2)
	if d < MIN_DISTANCE_TO_LIGHT and not is_terminated then
		is_terminated = true
		log("Step: ", n_step)
	end

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
