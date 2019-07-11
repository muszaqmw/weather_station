from Database import User
from pony import orm
from random import randrange
from flask_login import login_user
import Utils
from flask import redirect, abort, render_template


def handle(request):
    if request.method == 'POST':
        name = request.form['name']
        passwd = request.form['pass']
        user = User.User.get(name=name)

        if user is not None and Utils.encrypt_password(passwd) == user.password:
            print("done")
            user.ip = request.remote_addr
            user.token = randrange(2147483647)
            orm.commit()

            token = Utils.UserToken.UserToken(user.token)
            token.entity = user
            login_user(token)
            return redirect(request.args.get("next"))
        else:
            return abort(401)
    else:
        return render_template('login.html')