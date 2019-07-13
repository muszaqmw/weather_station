from flask import Flask, request
from flask_login import LoginManager, login_required, logout_user
from pony import orm
import Handlers
import Utils
from Config import Config
from Database import User, db


app = Flask(__name__, template_folder="Templates")
app.config.update(
    DEBUG=Config['DEBUG'],
    SECRET_KEY=Config['APP_KEY']
)

login_manager = LoginManager()
login_manager.init_app(app)
login_manager.login_view = "login"

orm.sql_debug(Config['DEBUG'])
db.generate_mapping(create_tables=True)


@app.route('/')
@login_required
def index():
    return Handlers.dashboardHandler(request)


@app.route("/Login", methods=["GET", "POST"])
@orm.db_session
def login():
    return Handlers.loginHandler(request)


@app.route("/Users")
@login_required
def showUsers():
    return Handlers.userShowHandler(request)


@app.route("/Users/Logout/<id>")
@login_required
def logoutUser(id):
    return Handlers.userLogoutHandler(request, id)

@app.route("/Users/Edit")
@app.route("/Users/Edit/<id>", methods=["GET", "POST"])
@login_required
def editUser(id = 0):
    return Handlers.userEditHandler(request, id)


@app.route("/Users/Remove/<id>", methods=["GET", "POST"])
@login_required
def removeUser(id = 0):
    return Handlers.userRemoveHandler(request, id)


@app.route("/Devices")
@login_required
def showDevices():
    return Handlers.deviceShowHandler(request)


@app.route("/Devices/Add", methods=["GET", "POST"])
@login_required
def addDevice():
    return Handlers.deviceAddHandler(request)


@app.route("/Devices/Remove/<id>", methods=["GET"])
@login_required
def removeDeivce(id):
    return Handlers.deviceRemoveHandler(request, id)


@app.route("/MeasuresTypes")
@login_required
def showMeasurementTypes():
    return Handlers.measurementTypeShowHandler(request)


@app.route("/MeasuresTypes/Add", methods=["GET", "POST"])
@login_required
def addMeasurementType():
    return Handlers.measurementTypeAddHandler(request)


@app.route("/MeasuresTypes/Remove/<id>", methods=["GET"])
@login_required
def removeMeasurementType(id):
    return Handlers.measurementTypeRemoveHandler(request, id)


@app.route("/Logout")
@login_required
def logout():
    logout_user()
    return "Logged out"


@app.errorhandler(401)
def page_error(e):
    return "Failed"


@login_manager.user_loader
@orm.db_session
def load_user(token):
    user = User.User.get(ip = request.remote_addr, token = token)
    if user is not None:
        userToken = Utils.UserToken.UserToken(token)
        userToken.entity = user
        return Utils.UserToken.UserToken(userToken)
    return None


if __name__ == "__main__":
    app.run()