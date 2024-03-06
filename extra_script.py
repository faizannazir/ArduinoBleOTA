Import("env")
import shutil
import os

def post_program_action(source, target, env):
# Get the custom_prog_version option from platformio.ini
    custom_prog_version = env.GetProjectOption("custom_prog_version")

# env.Replace(PROGNAME="firmware_%s" % env.GetProjectOption("custom_prog_version"))

# os.rename(".pio/build/esp32dev_nimble/firmware.bin",".pio/build/esp32dev_nimble/firmware_{custom_prog_version}.bin")
# Define the source and destination paths
    source_path = os.path.join(env.subst("$BUILD_DIR"), f"firmware.bin")
    print("-------------"+source_path)
    destination_path = os.path.join(env.subst("."), "release",f"firmware_{custom_prog_version}.bin")
    print(destination_path)

# Ensure the release folder exists
    os.makedirs(os.path.dirname(destination_path), exist_ok=True)

# Move the firmware file
    try:
        shutil.move(source_path, destination_path)
    except Exception as e:
        print(f"Error moving firmware file: {e}")
        
env.AddPostAction("$PROGPATH", post_program_action)