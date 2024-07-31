import requests
from pathlib import Path
from io import BytesIO

import pandas as pd
import streamlit as st
from PIL import Image
import plotly.express as px

st.set_page_config(page_title='Smart Attendance',
                   layout="wide")

# Define a flag to check if the submit button is clicked
submit_clicked = False

if __name__ == '__main__':
   
    filepath = Path(r'C:\Users\Akshata\OneDrive\Documents\Interactive Systems\Smart_Attendance_WebPage')

    tab1, tab2, tab3 = st.tabs(["Employee Profile", "Time Log Statistics", "Rewardees"])

    with tab1:
        st.header("Profile")
        col1,col2 = st.columns([1,2])
        with col1:
        # Using st.markdown with HTML
            st.markdown(f'<img src="{"https://www.viennashots.com/wp-content/uploads/2021/05/09_VS_Mitarbeiterportraits-e1559232425483.jpg"}" alt="Image" width=300>', unsafe_allow_html=True)

        with col2:
            st.markdown( "<h1 style='padding-left: 100px; text-align: left; font-size: 18px; font-family: Arial , sans-serif;'>Employee Name: Kaiden Jackson</h1>", unsafe_allow_html=True)
            st.markdown( "<h1 style='padding-left: 100px; text-align: left; font-size: 18px; font-family: Arial , sans-serif;'>Employee No: 103</h1>", unsafe_allow_html=True)
            st.markdown( "<h1 style='padding-left: 100px; text-align: left; font-size: 18px; font-family: Arial , sans-serif;'>Employee Department: Car</h1>", unsafe_allow_html=True)
            st.markdown( "<h1 style='padding-left: 100px; text-align: left; font-size: 18px; font-family: Arial , sans-serif;'>Profile Description: I am an enthusiastic, self-motivated, reliable, responsible and hard working person. I am a mature team worker and adaptable to all challenging situations. I am able to work well both in a team environment as well as using own initiative. I am able to work well under pressure and adhere to strict deadlines.", unsafe_allow_html=True)
            st.markdown( "<h1 style='padding-left: 100px; text-align: left; font-size: 18px; font-family: Arial , sans-serif;'>Streaks: 3 </h1>", unsafe_allow_html=True)

        # Add footer at the bottom of the page
        st.markdown('---')
        st.markdown("Image Reference: [Viennashots](https://www.viennashots.com/)")

    with tab2:
            # Set header with custom formatting
            st.markdown("<h1 style='text-align: left; font-size: 48px; font-family: Garamond, sans-serif; text-transform: uppercase; font-weight: bold; color: #F63366;'>SMART ATTENDANCE</h1>", unsafe_allow_html=True)
            st.subheader('Analysis')

            ### --- LOAD DATAFRAME
            excel_file = filepath.joinpath('Log_Output_details_DEMO.xlsx')
            sheet_name = 'DATA'

            df = pd.read_excel(excel_file,
                    sheet_name=sheet_name,
                    usecols='B:F',
                    header=1)

            df_streak = pd.read_excel(excel_file,
                            sheet_name= sheet_name,
                            usecols='L:M',
                            header=1)
            
            
            df_employees = pd.read_excel(excel_file,
                            sheet_name= sheet_name,
                            usecols='I:J',
                            header=1)
            
            df_streak = df_streak.rename(columns={'Streaks.1': 'Streaks/Day'})

            if not df.empty:
                # --- STREAMLIT SELECTION
                department = df['Department'].unique().tolist()
                ages = df['Employee ID'].unique().tolist()

                age_selection = st.slider('Employee ID',
                        min_value= min(ages),
                        max_value= max(ages),
                        value=(min(ages),max(ages)))

                department_selection = st.multiselect('Department:',
                                department,
                                default=department)

                # --- FILTER DATAFRAME BASED ON SELECTION
                mask = (df['Employee ID'].between(*age_selection)) & (df['Department'].isin(department_selection))


                number_of_result = df[mask].shape[0]
                st.markdown(f'*Available Results: {number_of_result}*')

                # --- GROUP DATAFRAME AFTER SELECTION
                df_grouped = df[mask].groupby(by=['Time']).count()[['Employee ID']]
                df_grouped = df_grouped.rename(columns={'Employee ID': 'Votes'})
                df_grouped = df_grouped.reset_index()

                st.markdown("<h2 style='font-size: 24px;'>Employee attendance log</h2>", unsafe_allow_html=True)

                if df[mask].empty:
                    st.markdown("*No results available for the selected filters.*")
                else:
                    # Display the image and dataframe
                    col3, col4 = st.columns(2)
                    image = Image.open(filepath.joinpath('Cover Photo.png'))
                    col3.image(image,
                            caption='Smart Attendance System',
                            use_column_width=True)
                    col4.dataframe(df[mask])
                
            col5, col6 = st.columns(2, gap= "medium")
            with col5:
                
                if not df_employees.empty:
                    # --- PLOT BAR CHART
                    bar_chart_1 = px.bar(df_employees,
                                x='Name.1',
                                y='E_Streaks',
                                text='E_Streaks',
                                color_discrete_sequence = ['#F63366']*len(df),
                                template= 'plotly_white')
                    
                    bar_chart_1.update_layout(
                        xaxis=dict(
                            type='category',
                            tickmode='linear',
                            ticktext=df_employees['Name.1'],
                            tickvals=df_employees['Name.1'],
                            title='Employee Name'
                        ),
                        yaxis_title='Streaks',
                        font=dict(size=18),
                        autosize=False,
                        width=750,  # Adjust the width of the figure as per your requirement
                        height=600  # Adjust the height of the figure as per your requirement
                    )
                    bar_chart_1.update_traces(textposition='outside')

                    st.plotly_chart(bar_chart_1)

                    st.markdown("<h1 style='text-align: center; font-size: 24px; color: #F63366;'>Employees Streaks</h1>", unsafe_allow_html=True)

            with col6:

                if not df_streak.empty:
                    # --- PLOT PIE CHART
                    pie_chart_1 = px.pie(df_streak,
                            values='D_Streaks',
                            names='Dept')

                    st.plotly_chart(pie_chart_1)
                    st.markdown("<h1 style='text-align: center; font-size: 24px; color: #F63366;'>Department Streaks</h1>", unsafe_allow_html=True)

            # Add spacing between the plots
            st.markdown('\n\n')
            st.markdown('\n\n')

    with tab3:

            # Calculate streaks for each department
            df_department_streaks = df_streak.groupby('Dept')['D_Streaks'].sum().reset_index()

            # Sort department streaks in descending order
            df_department_streaks = df_department_streaks.sort_values('D_Streaks', ascending=False)

            # Extract top three departments
            top_three_departments = df_department_streaks.head(3)

            col7,col8 = st.columns([1,2])

            with col7:
                # Apply custom CSS styles to the expander title
                expander_style = """
                                    <style>
                                        .streamlit-expanderHeader {
                                            color: #F63366;
                                            font-size: 20px;
                                            font-weight: bold;
                                            font-family: "Garamond", sans-serif;
                                        }
                                    </style>
                                    """
                st.markdown(expander_style, unsafe_allow_html=True)

                # Create the expander for the section
                with st.expander("Top Three Departments"):
                        if not top_three_departments.empty:
                            for index, row in top_three_departments.iterrows():
                                department_name = row['Dept']
                                medal_symbol = "🥇" if index == 0 else "🥈" if index == 1 else "🥉"  # Assign medal symbols based on index
                                st.write(f"{medal_symbol} {department_name}")
                        else:
                            st.markdown("Insufficient data to cummulate top three departments!!")

                # Calculate streaks for each employee
                df_employee_streaks = df_employees.groupby('Name.1')['E_Streaks'].sum().reset_index()

                # Sort employee streaks in descending order
                df_employee_streaks = df_employee_streaks.sort_values('E_Streaks', ascending=False)

                # Extract top three employees
                top_three_employees = df_employee_streaks.head(3)
                # Create the expander for the section
                with st.expander("Top Three Employees"):
                    if not top_three_employees.empty:
                        for index, row in top_three_employees.iterrows():
                            employee_name = row['Name.1']
                            medal_symbol = "🥇" if index == 0 else "🥈" if index == 1 else "🥉"  # Assign medal symbols based on index
                            st.write(f"{medal_symbol} {employee_name}")
                    else: 
                        st.markdown("Insufficient data to cummulate top three departments")

                # Define the list of coupons with their image URLs and descriptions
                coupons = [
                    {
                        'image_url': 'https://cdn.zoutons.com/images/originals/blog/amazon_1681817074.png',
                    },
                    {
                        'image_url': 'https://cdn.zoutons.com/images/originals/blog/1677155590364.jpg_1677155591.png',
                    },
                    {
                        'image_url': 'https://cdn.zoutons.com/images/originals/blog/MAIN-BANNER_1675847133.png',
                    }
                ]

                # Display coupons with image and description in separate columns
                st.header("Coupons")

                # Loop through the coupons
                for coupon in coupons:
                    response = requests.get(coupon['image_url'])
                    image = Image.open(BytesIO(response.content))

                    # Display the resized image in the first column
                    st.image(image, use_column_width=True)

                    # Add a separator between coupons
                    st.markdown("---")

            with col8:

                # Read the login times from the Excel file
                df = pd.read_excel(excel_file, sheet_name=sheet_name, usecols='D,E', header=1)
                df['Time'] = pd.to_datetime('1900-01-01 ' + df['Time'].astype(str))  # Combine dummy date with time  # Convert login time column to datetime format

                # Group the data by employee and count the number of logins before 09:00:00
                early_bird_df = df[df['Time'].dt.time < pd.to_datetime('09:00:00').time()]
                early_bird_counts = early_bird_df.groupby('Name')['Time'].count()

                if early_bird_counts.empty:
                    EarlyBird_employee = None
                
                else:              
                    # Find the employee with the highest login count before 09:00:00
                    EarlyBird_employee = early_bird_counts.idxmax()               


                # Assuming you have a DataFrame called 'df_streak' with streak data
                employee_perfect_attendance = None
                Attendance_Ace = None

                # Get the total number of working days in the month
                total_working_days = pd.bdate_range(start=pd.to_datetime('today').replace(day=1), end=pd.to_datetime('today')).shape[0]
                
                for index, row in df_employees.iterrows():
                    # Check if the streak matches the number of working days in the month
                    if row['E_Streaks'] >= total_working_days:
                        Attendance_Ace = row['Name.1']
                        break  # Exit the loop after finding the first employee with perfect attendance

                top_streak_employee = df_employee_streaks.head(1)
                Streak_Star_Accolade = row['Name.1']

                Cohesive_ContinuityCrown = df_department_streaks.iloc[0]['Dept']
                
                awards = ["Early Bird", "Attendance Ace", "Streak Star Accolade", "Cohesive Continuity Crown"]

                TOP_EMPLOYEES = [ EarlyBird_employee, Attendance_Ace, Streak_Star_Accolade, Cohesive_ContinuityCrown ]

                # Unicode characters representing the badges
                badge_symbols = ["🏆", "🌟", "⭐", "👑"]

                # Create a dictionary with the lists as values and column names as keys
                data = {
                    'Awards': [f"{badge_symbols[i]} {awards[i]}" for i in range(len(awards))],
                    'Top Employee/Department': TOP_EMPLOYEES,
                }

                # Custom heading using Markdown formatting
                heading = "<h2 style='text-align: center; color: #F63366; font-size: 24px'>Badges Honour</h2>"

                # Display the heading using Markdown
                st.markdown(heading, unsafe_allow_html=True)

                # Create a DataFrame using the dictionary
                df = pd.DataFrame(data)

                # Display the DataFrame
                table = st.table(df)

                # Define reward points for each award
                reward_points = {
                    "Early Bird": 500,
                    "Attendance Ace": 800,
                    "Streak Star Accolade": 1000,
                    "Cohesive Continuity Crown": 1500
                }

                # Custom heading using Markdown formatting
                heading = "<h2 style='text-align: center; color: #F63366; font-size: 24px'>Claim Rewards</h2>"

                # Display the heading using Markdown
                st.markdown(heading, unsafe_allow_html=True)

                # Get the selected award from the user
                selected_award = st.selectbox("Select an award:", awards)

                # Get the points associated with the selected award
                reward_points_selected = reward_points[selected_award]

                # Get the employee name from the user
                employee_name = st.text_input("Enter your name:")

                # Center-align the submit button
                col9, col10 = st.columns([0.48,0.62])
                with col9:
                    pass
                with col10:
                    submit_clicked = st.button("Submit")

                if submit_clicked:
                    if employee_name:
                        # Validate the employee name
                        valid_employee = employee_name in TOP_EMPLOYEES

                        # Calculate the total points required for the selected award
                        total_points_required = reward_points_selected

                        # Check if the employee has enough points to claim the reward
                        if valid_employee and TOP_EMPLOYEES.index(employee_name) <= awards.index(selected_award):
                            st.success(f"Congratulations, {employee_name}! You have earned {reward_points_selected} points and are eligible to claim the {selected_award} reward.")
                            st.write("Please contact the HR department to proceed with the reward claim.")
                        elif valid_employee:
                            st.warning(f"Sorry, {employee_name}. You do not have enough points to claim the {selected_award} reward. Keep up the good work and accumulate more points!")
                        else:
                            st.error("Invalid employee name. Please enter a valid name.")










                