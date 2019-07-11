from pony import orm
from flask import redirect, url_for
from Database import User


@orm.db_session
def handle(request, userId):
    if userId and userId.isnumeric():
        User.User[userId].delete()
        orm.commit()

    return redirect(url_for("showUsers"))
