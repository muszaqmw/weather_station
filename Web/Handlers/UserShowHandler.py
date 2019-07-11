from pony import orm
from Database import User
from flask import render_template


@orm.db_session
def handle(request):
    users = orm.select(d for d in User.User)

    return render_template('users.html', users=users)