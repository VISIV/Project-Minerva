package russmanilads.projectminerva;

import android.graphics.Typeface;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

public class Login extends AppCompatActivity {
    TextView regular;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        regular = (TextView) findViewById(R.id.username);
        Typeface UIFont= Typeface.createFromAsset(getAssets(),"fonts/segoeui.ttf");
        regular.setTypeface(UIFont);
    }
}
