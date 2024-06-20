Import("env")

print("Current CLI targets", COMMAND_LINE_TARGETS)
print("Current Build targets", BUILD_TARGETS)

def post_program_action(source, target, env):
    program_path = target[0].get_abspath()
    no_ext_program_path = program_path.rsplit('.bin')[0]
    env.Execute(
        "python "
        "~/.platformio/packages/framework-arduinoespressif8266/tools/signing.py "
        "--mode sign "
        "--privatekey ./ssl/private.key "
        f"--bin {program_path} "
        f"--out {no_ext_program_path}.signed.bin"
    )
    print("Binary signed!")

env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", post_program_action)
