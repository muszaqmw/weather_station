from flask_login import UserMixin


class UserToken(UserMixin):
    def __init__(self, id):
        self.id = id
        self.entity = None