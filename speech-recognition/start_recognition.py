import snowboydecoder
import signal
import os
import json

COMMAND_MAPPING_FILE = "command_mapping.json"
interrupted = False


def interrupt_handler(signal, frame):
    global interrupted
    interrupted = True


def interrupt_check_callback():
    global interrupted
    return interrupted


def process_config_file():
    with open(COMMAND_MAPPING_FILE) as mapping_file:
        config = json.load(mapping_file)
        mappings = config["commandMapping"]

        for mapping in mappings:
            # print(mapping)
            hotwords.append(mapping["hotword"])
            callbacks.append(lambda: os.system(mapping["command"]))


def validate_hotwords():
    error = False
    for hotword in hotwords:
        if not os.path.isfile(hotword):
            print("No such file: {}".format(hotword))
            error = True

    if error:
        raise IOError("Required file does not exist")


callbacks = []
hotwords = []

try:
    process_config_file()
    validate_hotwords()
except EnvironmentError as e:
    print("""Error during processing config file.
        Make sure that {} exists in the project's folder
        and the model files exist for the hotwords""".format(
        COMMAND_MAPPING_FILE))
    quit()


signal.signal(signal.SIGINT, interrupt_handler)

sensitivity = [0.5] * len(hotwords)
detector = snowboydecoder.HotwordDetector(hotwords, sensitivity=sensitivity)
print('Listening... Press Ctrl+C to exit')
print('hotwords: {}'.format(hotwords))

detector.start(detected_callback=callbacks,
               interrupt_check=interrupt_check_callback,
               sleep_time=0.03)

detector.terminate()
