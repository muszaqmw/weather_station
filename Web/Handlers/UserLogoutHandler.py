from pony import orm
from flask import redirect, url_for
from Database import User


@orm.db_session
def handle(request, userId):
    if userId and userId.isnumeric():
        user = User.User.get(id=userId)
        user.token = 0
        user.ip = "logged out"
        orm.commit()

    return redirect(url_for("showUsers"))
