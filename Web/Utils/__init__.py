import hashlib
from . import UserToken


def encrypt_password(password):
    return hashlib.sha3_256(password.encode('utf-8')).hexdigest()
