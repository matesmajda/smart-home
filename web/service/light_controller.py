import os

controller_program = "sudo /home/pi/Downloads/milight/mi_remote"

rooms = ["living-room", "bedroom"]
types = ["white", "rgb"]
commands = ["on", "off", "brightnessUp", "brightnessDown"]


def control_light(room, type, command):
    validate(room, type, command)
    params = get_params(room, type, command)
    execute_controller(params)
    return True


def get_params(room, type, command):
    return "-r {} -t {} -b {}".format(room, type, command)


def execute_controller(params):
    cmd = controller_program + " " + params
    print("executing {}".format(cmd))
    try:
        os.system(cmd)
    except Exception as e:
        print("failed to execute command: '{}'. Exception is: {}".format(cmd, e))


def validate(room, type, command):
    if room not in rooms:
        raise ValueError("invalid room " + room)
    if type not in types:
        raise ValueError("invalid type " + type)
    if command not in commands:
        raise ValueError("invalid command " + command)
