# protoc --proto_path=. -I . --python_out=. --pyi_out=. response.proto
from proto import user_connection_pb2
from proto import chat_pb2
import os

test_name = "[Регистрация пользователя]"
program_path = "../build/Speaky"
input_file_path = "register_user/register_user_input.txt"
input_file_path_serialized = "register_user/register_user_input_serialized.txt"
output_file_path = "register_user/register_user_output.txt"

print(f"Запуск теста:\t{test_name}")

try:
    with open(input_file_path, "r") as file:
        input_name = file.read()

    with open(input_file_path_serialized, "wb") as file:
        user = user_connection_pb2.User(name=input_name)
        file.write(user.SerializeToString())

    os.system(
        f"{program_path} {input_file_path_serialized} {output_file_path}")

    with open(output_file_path, "rb") as file:
        chat = chat_pb2.Chat()
        chat.ParseFromString(file.read())
        assert chat.messages[0].user_name == input_name

    print(f"Тест:\t\t\t{test_name}: ПРОЙДЕН")
except:
    print(f"Тест:\t\t\t{test_name}: НЕ ПРОЙДЕН")
