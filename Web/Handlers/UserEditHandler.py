from flask import render_template, url_for, redirect
from Database import User
from pony import orm
import Utils


@orm.db_session
def handle(request, userId):
    user = None
    if userId != 0:
        user = User.User.get(id = userId)

    if user is not None and request.method == 'POST':
        user.name = request.form['name']
        user.password = Utils.encrypt_password(request.form['password'])
        user.token = 0
        user.ip = "edited"
        orm.commit()
        return redirect(url_for('showUsers'))

    if user is None and request.method == 'POST':
        User.User(name=request.form['name'], password=Utils.encrypt_password(request.form['password']), token=0, ip="new")
        return redirect(url_for('showUsers'))

    return render_template('editUser.html', user=user)