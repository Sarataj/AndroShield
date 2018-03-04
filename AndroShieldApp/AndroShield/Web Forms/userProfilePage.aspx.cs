﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace AndroShield.Web_Forms
{
    public partial class userProfilePage : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            ClientScript.GetPostBackEventReference(this, string.Empty);
            userEmail.Text = Session["username"].ToString();
        }
        protected void Profile(object sender, EventArgs e)
        {
            Response.Redirect("userProfilePage.aspx");
        }
        protected void Logout(object sender, EventArgs e)
        {
            Session["username"] = "";
            Session["userAccount"] = "";
            Response.Redirect("homePage.aspx");
        }

    }
}