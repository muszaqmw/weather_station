import hashlib
from . import UserToken
import random
import string


def encrypt_password(password):
    return hashlib.sha3_256(password.encode('utf-8')).hexdigest()


def random_chars(length):
    return ''.join(random.choices(string.ascii_uppercase + string.digits, k=length))