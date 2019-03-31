import unittest
import random
import string
import hashlib

import sha1


CNT_ROUNDS = 80


class TestSha1(unittest.TestCase):
    def test_repeatable(self):
        print('\n>>> test_repeatable')
        msg = get_random_string()

        first_digest = sha1.sha1(msg, CNT_ROUNDS)
        second_digest = sha1.sha1(msg, CNT_ROUNDS)

        print('... test_repeatable: checking for identical digests')
        self.assertEqual(first_digest, second_digest)
        print('... test_repeatable: success')

    def test_comparison(self):
        print('\n>>> test_comparison')
        msg = get_random_string()

        custom_sha1 = sha1.sha1(msg, CNT_ROUNDS)
        lib_sha1 = hashlib.sha1(msg.encode()).hexdigest()

        print('... test_comparison: checking for identical digests')
        self.assertEqual(custom_sha1, lib_sha1)
        print('... test_comparison: success')


def get_random_string():
    return ''.join(random.choice(string.ascii_letters + string.digits) for _ in range(random.randint(10, 10 ** 5)))


if __name__ == '__main__':
    unittest.main()