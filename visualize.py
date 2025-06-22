import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches


income_df = pd.read_csv("income_report.csv")
expense_df = pd.read_csv("expense_report.csv")


income_df['Date'] = pd.to_datetime(income_df['Date'], format='%d-%b-%y', errors='coerce')
expense_df['Date'] = pd.to_datetime(expense_df['Date'], format='%d-%b-%y', errors='coerce')
income_df.dropna(subset=['Date'], inplace=True)
expense_df.dropna(subset=['Date'], inplace=True)


income_df['Period'] = income_df['Date'].dt.to_period('M')
expense_df['Period'] = expense_df['Date'].dt.to_period('M')


print("Choose a visualization option:")
print("1. Visualize a specific month's expenses as a pie chart (with remaining money)")
print("2. Visualize total expenses per month as a bar chart")
print("3. Visualize total expenses per category across all months")
choice = input("Enter 1, 2 or 3: ").strip()


if choice == "1":
    month_input = input("Enter month and year (e.g., May 2025): ").strip()
    try:
        month_year = pd.to_datetime(f"01 {month_input}", format='%d %B %Y')
        target_period = month_year.to_period('M')
    except ValueError:
        print(" Invalid date format. Use format like 'May 2025'.")
        exit()

  
    monthly_income = income_df[income_df['Period'] == target_period]['Amount'].sum()
    monthly_expenses = expense_df[expense_df['Period'] == target_period]
    expense_by_category = monthly_expenses.groupby('Category')['Amount'].sum()

    total_expense = expense_by_category.sum()
    remaining = max(0, monthly_income - total_expense)

   
    plot_data = expense_by_category.copy()
    if remaining > 0:
        plot_data['Remaining'] = remaining

    def format_label(pct, allvals):
        absolute = int(round(pct/100.*sum(allvals)))
        return f"₹{absolute:,}\n({pct:.1f}%)"

    colors = plt.cm.Pastel2.colors
    fig, ax = plt.subplots(figsize=(10, 8))
    wedges, texts, autotexts = ax.pie(
        plot_data,
        labels=plot_data.index,
        autopct=lambda pct: format_label(pct, plot_data),
        startangle=140,
        colors=colors,
        pctdistance=0.8,
        wedgeprops=dict(width=0.3, edgecolor='w')
    )

    plt.text(0, 0, f"Salary\n₹{int(monthly_income):,}", ha='center', va='center', fontsize=14, fontweight='bold')

    legend_labels = [f"{cat}: ₹{amt:,.0f}" for cat, amt in plot_data.items()]
    legend_patches = [mpatches.Patch(color=colors[i % len(colors)], label=legend_labels[i]) for i in range(len(plot_data))]
    ax.legend(handles=legend_patches, loc='center left', bbox_to_anchor=(1, 0.5), title="Categories", fontsize=10)

    plt.title(f"📊 Monthly Expense Breakdown – {month_input}", fontsize=16, fontweight='bold', pad=20)
    plt.axis('equal')
    plt.tight_layout()
    plt.show()


elif choice == "2":
    monthly_expense = expense_df.groupby(expense_df['Date'].dt.to_period('M'))['Amount'].sum()
    monthly_expense.index = monthly_expense.index.strftime('%b %Y')

    plt.figure(figsize=(10, 6))
    plt.plot(monthly_expense.index, monthly_expense.values, marker='o', linestyle='-', color='tomato', label='Expense')
    
    for i, val in enumerate(monthly_expense.values):
        plt.text(i, val + 200, f"₹{int(val)}", ha='center', fontsize=9)

    plt.title(" Total Monthly Expenses Over Time", fontsize=15, fontweight='bold')
    plt.xlabel("Month", fontsize=12)
    plt.ylabel("Expense Amount (INR)", fontsize=12)
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.xticks(rotation=45)
    plt.tight_layout()
    plt.legend()
    plt.show()



elif choice == "3":
    unique_categories = sorted(expense_df['Category'].dropna().unique())
    print("\nAvailable categories:")
    for cat in unique_categories:
        print(f"- {cat}")

    user_category = input("\nEnter the category you want to visualize (case-insensitive): ").strip().lower()
    expense_df['Category_lower'] = expense_df['Category'].str.lower()

    if user_category not in expense_df['Category_lower'].values:
        print(f" Category '{user_category}' not found in data.")
    else:
        filtered = expense_df[expense_df['Category_lower'] == user_category]
        monthly_expense = filtered.groupby(filtered['Date'].dt.to_period('M'))['Amount'].sum()
        monthly_expense.index = monthly_expense.index.strftime('%b %Y')

        plt.figure(figsize=(10, 6))
        plt.plot(monthly_expense.index, monthly_expense.values, marker='o', linestyle='-', color='dodgerblue')

        for i, val in enumerate(monthly_expense.values):
            plt.text(i, val + 200, f"₹{int(val)}", ha='center', fontsize=9)

        plt.title(f" Monthly Expense for Category: {user_category.title()}", fontsize=15, fontweight='bold')
        plt.xlabel("Month", fontsize=12)
        plt.ylabel("Amount (INR)", fontsize=12)
        plt.grid(True, linestyle='--', alpha=0.6)
        plt.xticks(rotation=45)
        plt.tight_layout()
        plt.show()



