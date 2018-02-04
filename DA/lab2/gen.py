# -*- coding: utf-8 -*-
import sys
import random
from string import ascii_letters

def get_random_key():
    #return random.choice( string.ascii_letters )
    kek = random.randint(1,10)
    return ''.join(random.choice(ascii_letters) for _ in range(kek))

if __name__ == "__main__":
    # Проверяем, что передали 1 аргумент.
    #if len(sys.argv) != 2:
    #    print( "Usage: {0} <count of tests>".format( sys.argv[0] ) )
    #    sys.exit(1)
    
    #count_of_tests = int( sys.argv[1] )
    count_of_tests = 1

    actions = [ "+", "-"]
    acts_file = ["Load test", "Save test"]

    for enum in range( count_of_tests ):
        keys = dict()
        test_file_name = "tests/{:02d}".format( 6 )
        with open( "{0}.t".format( test_file_name ), 'w' ) as output_file, \
             open( "{0}.a".format( test_file_name ), "w" ) as answer_file:

            # Для каждого файла генерируем от 1 до 100 тестов.
            for x in range(500000):
                if x % 5 == 0:
                    action = "-"
                else:
                    action = "+"
                #action = random.choice( actions )
                if action == "+":
                    key = get_random_key()
                    value = random.randint( 1, 1000000 )
                    output_file.write("+ {0} {1}\n".format( key, value ))
                    key = key.lower()
                    # Если в нашем словаре уже есть такой ключ, то ответе должно быть 
                    # сказано, что он существует, иначе --- успешное добавление.
                    answer = "Exist"
                    if key not in keys:
                        answer = "OK"
                        keys[key] = value
                    answer_file.write( "{0}\n".format( answer ) )

                elif action == "?":
                    search_exist_element = random.choice( [ True, False ] )
                    key = random.choice( [ key for key in keys.keys() ] ) if search_exist_element and len( keys.keys() ) > 0 else get_random_key()
                    output_file.write( "{0}\n".format( key ) )
                    key = key.lower()
                    if key in keys:
                        answer = "OK: {0}".format( keys[key] )
                    else:
                        answer = "NoSuchWord"
                    answer_file.write( "{0}\n".format( answer ) )

                elif action == '-':
                    del_exist_element = True
                    if del_exist_element == True and len( keys.keys() ) > 0:
                        key = random.choice( [ key for key in keys.keys() ] )
                    else:
                        key = get_random_key()
                    #key = get_random_key()
                    output_file.write("- {0}\n".format(key))
                    key = key.lower()
                    answer = "NoSuchWord"
                    if key in keys:
                        del keys[key]
                        answer = "OK"
                    answer_file.write("{0}\n".format( answer ) )

                #elif action == "!":
                #    act_file = random.choice(acts_file)
                #    if act_file == "Save test":
                #        output_file.write("{0} {1}\n".format( actionm act_file ))
                #        save_file = keys.copy()
                #        answer = "OK"
