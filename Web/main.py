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